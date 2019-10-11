#define UMDF_USING_NTSTATUS
#include <tchar.h>
#include <stdio.h>
#include <ntstatus.h>
#include "WssService.hpp"
#include <winternl.h>
#include <TlHelp32.h>
#include "WscResourceWrapper.hpp"
#include "WscResourceTraitsWin32.hpp"
#include "WscRuntime.hpp"
#include <system_error>

#ifdef _M_IX86
#include "WindowsSudoRpcX86.h"
#endif

#ifdef _M_X64
#include "WindowsSudoRpcX64.h"
#endif

#pragma comment(lib, "ntdll")
#pragma comment(lib, "rpcrt4")

namespace WSS::Internal {

    static SERVICE_STATUS_HANDLE gs_ServiceStatusHandle;

    DWORD ServiceSetStatus(_In_ DWORD dwCurrentState, _In_ DWORD dwControlsAccepted, _In_ DWORD dwWin32ExitCode) {
        DWORD dwErrorCode = ERROR_SUCCESS;
        SERVICE_STATUS ServiceStatus = {};

        ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
        ServiceStatus.dwCurrentState = dwCurrentState;
        ServiceStatus.dwControlsAccepted = dwControlsAccepted;
        ServiceStatus.dwWin32ExitCode = dwWin32ExitCode;
        ServiceStatus.dwServiceSpecificExitCode = 0;
        ServiceStatus.dwCheckPoint = 0;
        ServiceStatus.dwWaitHint = 0;

        if (::SetServiceStatus(gs_ServiceStatusHandle, &ServiceStatus)) {
            return ERROR_SUCCESS;
        } else {
            return GetLastError();
        }
    }

    static DWORD WINAPI ServiceHandlerEx(_In_ DWORD dwControl, _In_ DWORD dwEventType, _In_ LPVOID lpEventData, _In_ LPVOID lpContext) {
        RPC_STATUS RpcStatus;

        switch (dwControl) {
            case SERVICE_CONTROL_INTERROGATE:
                return NO_ERROR;
            case SERVICE_CONTROL_SHUTDOWN:
            case SERVICE_CONTROL_STOP:
                RpcStatus = RpcMgmtStopServerListening(NULL);
                if (RpcStatus != RPC_S_OK) {
                    return ServiceSetStatus(SERVICE_STOPPED, 0, RpcStatus);
                }

                RpcStatus = RpcServerUnregisterIf(NULL, NULL, FALSE);
                if (RpcStatus != RPC_S_OK) {
                    return ServiceSetStatus(SERVICE_STOPPED, 0, RpcStatus);
                }

                if (g_hLsassProcessToken) {
                    CloseHandle(g_hLsassProcessToken);
                    g_hLsassProcessToken = NULL;
                }

                return ServiceSetStatus(SERVICE_STOPPED, 0, ERROR_SUCCESS);
            default:
                return ERROR_CALL_NOT_IMPLEMENTED;
        }
    }

    VOID WINAPI ServiceMain(_In_ DWORD dwNumServicesArgs, _In_ PTSTR lpServiceArgVectors[]) {
        try {
            gs_ServiceStatusHandle = RegisterServiceCtrlHandlerExW(WINDOWS_SUDO_SERVICE_NAME, ServiceHandlerEx, NULL);
            if (gs_ServiceStatusHandle == NULL) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }

            //
            // Set current directory to directory where WindowsSudo.Service.exe exists.
            //
            {
                NTSTATUS NtStatus;
                ULONG ReturnLength;
                WSC::ResourceWrapper<WSC::ResourceTraitsHeapAlloc> lpusImagePath;

                //
                // ((PROCESSINFOCLASS)43) is ProcessImageFileNameWin32
                //
                NtStatus = NtQueryInformationProcess(GetCurrentProcess(), (PROCESSINFOCLASS)43, lpusImagePath.GetAddressOf(), sizeof(PVOID), &ReturnLength);
                if (NtStatus != STATUS_INFO_LENGTH_MISMATCH && !NT_SUCCESS(NtStatus)) {
                    throw std::system_error(RtlNtStatusToDosError(NtStatus), std::system_category());
                }

                lpusImagePath.TakeOver(HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ReturnLength));
                if (lpusImagePath.IsValid() == false) {
                    auto err = GetLastError();
                    throw std::system_error(err, std::system_category());
                }

                //
                // ((PROCESSINFOCLASS)43) is ProcessImageFileNameWin32
                //
                NtStatus = NtQueryInformationProcess(GetCurrentProcess(), (PROCESSINFOCLASS)43, lpusImagePath, ReturnLength, &ReturnLength);
                if (!NT_SUCCESS(NtStatus)) {
                    throw std::system_error(RtlNtStatusToDosError(NtStatus), std::system_category());
                }

                for (int i = lpusImagePath.As<PUNICODE_STRING>()->Length / 2 - 1; i >= 0; --i) {
                    switch (lpusImagePath.As<PUNICODE_STRING>()->Buffer[i]) {
                        case L'\\':
                        case L'/':
                            i = 0;
                            break;
                        default:
                            lpusImagePath.As<PUNICODE_STRING>()->Buffer[i] = 0;
                            break;
                    }
                }

                if (SetCurrentDirectoryW(lpusImagePath.As<PUNICODE_STRING>()->Buffer) == FALSE) {
                    auto err = GetLastError();
                    throw std::system_error(err, std::system_category());
                }
            }

            //
            // Finding token of lsass.exe
            //
            {
                WSC::ResourceWrapper<WSC::ResourceTraitsGenericHandle> hSnapshot;
                PROCESSENTRY32 ProcessEntry = { sizeof(PROCESSENTRY32) };

                hSnapshot.TakeOver(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));
                if (hSnapshot.IsValid() == false) {
                    auto err = GetLastError();
                    throw std::system_error(err, std::system_category());
                }

                if (Process32First(hSnapshot, &ProcessEntry) == FALSE) {
                    auto err = GetLastError();
                    throw std::system_error(err, std::system_category());
                }

                do {
                    if (_tcsicmp(ProcessEntry.szExeFile, TEXT("lsass.exe")) == 0) {
                        WSC::ResourceWrapper<WSC::ResourceTraitsGenericHandle> hProcess;
                        WSC::ResourceWrapper<WSC::ResourceTraitsGenericHandle> hProcessToken;

                        hProcess.TakeOver(OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, ProcessEntry.th32ProcessID));
                        if (hProcess.IsValid() == false) {
                            auto err = GetLastError();
                            throw std::system_error(err, std::system_category());
                        }

                        if (OpenProcessToken(hProcess, TOKEN_ALL_ACCESS, hProcessToken.GetAddressOf()) == FALSE) {
                            auto err = GetLastError();
                            throw std::system_error(err, std::system_category());
                        }

                        if (WSC::RtlTokenCheckPrivilege(hProcessToken, SE_CREATE_TOKEN_NAME, NULL)) {
                            g_hLsassProcessToken = hProcessToken.Transfer();
                            break;
                        }
                    }
                } while (Process32Next(hSnapshot, &ProcessEntry));

                if (g_hLsassProcessToken == NULL) {
                    throw std::system_error(ERROR_ASSERTION_FAILURE, std::system_category());
                }
            }

            //
            // Register RPC interface
            //
            RPC_STATUS RpcStatus = RpcServerUseAllProtseqsIf(0, WindowsSudoRpc_v1_0_s_ifspec, NULL);
            if (RpcStatus != RPC_S_OK) {
                throw std::system_error(RpcStatus, std::system_category());
            }

            RpcStatus = RpcServerRegisterIf(WindowsSudoRpc_v1_0_s_ifspec, NULL, NULL);
            if (RpcStatus != RPC_S_OK) {
                throw std::system_error(RpcStatus, std::system_category());
            }

            RpcStatus = RpcServerListen(1, RPC_C_LISTEN_MAX_CALLS_DEFAULT, TRUE);
            if (RpcStatus != RPC_S_OK) {
                throw std::system_error(RpcStatus, std::system_category());
            }

            ServiceSetStatus(SERVICE_RUNNING, SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_STOP, ERROR_SUCCESS);
        } catch (std::system_error& e) {
            ServiceSetStatus(SERVICE_STOPPED, 0, e.code().value());
        }
    }
}

extern "C" {

    _Must_inspect_result_
    _Ret_maybenull_ _Post_writable_byte_size_(size)
    void* __RPC_USER MIDL_user_allocate(_In_ size_t size) {
        return malloc(size);
    }

    void __RPC_USER MIDL_user_free(_Pre_maybenull_ _Post_invalid_ void* ptr) {
        free(ptr);
    }

}
