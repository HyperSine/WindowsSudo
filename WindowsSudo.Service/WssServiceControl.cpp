#define UMDF_USING_NTSTATUS
#include <ntstatus.h>
#include <windows.h>
#include <winternl.h>
#include <system_error>
#include "WscResourceWrapper.hpp"
#include "WscResourceTraitsWin32.hpp"
#include "WssService.hpp"

#pragma comment(lib, "ntdll")

namespace WSS {

    void ServiceInstall() {
        NTSTATUS NtStatus;

        ULONG ReturnLength;
        WSC::ResourceWrapper<WSC::ResourceTraitsHeapAlloc> lpusImagePath;

        WSC::ResourceWrapper<WSC::ResourceTraitsServiceHandle> ScManager;
        WSC::ResourceWrapper<WSC::ResourceTraitsServiceHandle> NewService;

        //
        // ((PROCESSINFOCLASS)43) is ProcessImageFileNameWin32
        //
        NtStatus = NtQueryInformationProcess(GetCurrentProcess(), (PROCESSINFOCLASS)43, lpusImagePath.GetAddressOf(), sizeof(PVOID), &ReturnLength);
        if (NtStatus != STATUS_INFO_LENGTH_MISMATCH && !NT_SUCCESS(NtStatus)) {
            auto err = RtlNtStatusToDosError(NtStatus);
            throw std::system_error(err, std::system_category());
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
            auto err = RtlNtStatusToDosError(NtStatus);
            throw std::system_error(err, std::system_category());
        }

        ScManager.TakeOver(OpenSCManagerW(NULL, SERVICES_ACTIVE_DATABASEW, SC_MANAGER_ALL_ACCESS));
        if (ScManager.IsValid() == false) {
            auto err = GetLastError();
            throw std::system_error(err, std::system_category());
        }

        NewService.TakeOver(
            CreateServiceW(
                ScManager,
                WINDOWS_SUDO_SERVICE_NAME,
                WINDOWS_SUDO_SERVICE_DISPLAYNAME,
                SERVICE_ALL_ACCESS,
                SERVICE_WIN32_OWN_PROCESS,
                SERVICE_AUTO_START,
                SERVICE_ERROR_NORMAL,
                lpusImagePath.template As<PUNICODE_STRING>()->Buffer,
                NULL,
                NULL,
                L"RpcSs\x00\x00",
                NULL,
                NULL)
        );
        if (NewService.IsValid() == false) {
            auto err = GetLastError();
            throw std::system_error(err, std::system_category());
        }
    }

    void ServiceStart() {
        WSC::ResourceWrapper<WSC::ResourceTraitsServiceHandle> ScManager;
        WSC::ResourceWrapper<WSC::ResourceTraitsServiceHandle> TargetService;

        ScManager.TakeOver(OpenSCManagerW(NULL, SERVICES_ACTIVE_DATABASEW, SC_MANAGER_ALL_ACCESS));
        if (ScManager.IsValid() == false) {
            auto err = GetLastError();
            throw std::system_error(err, std::system_category());
        }

        TargetService.TakeOver(OpenServiceW(ScManager, WINDOWS_SUDO_SERVICE_NAME, SERVICE_ALL_ACCESS));
        if (TargetService.IsValid() == false) {
            auto err = GetLastError();
            throw std::system_error(err, std::system_category());
        }

        BOOL StartRequested = FALSE;
        SERVICE_STATUS TargetServiceStatus = {};
        for (auto TimeStart = GetTickCount64(); TargetServiceStatus.dwCurrentState != SERVICE_RUNNING && GetTickCount64() - TimeStart < 3000; Sleep(1000)) {
            if (QueryServiceStatus(TargetService, &TargetServiceStatus) == FALSE) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }

            if (TargetServiceStatus.dwCurrentState == SERVICE_RUNNING) {
                break;
            }

            if (TargetServiceStatus.dwCurrentState != SERVICE_START_PENDING && StartRequested == FALSE) {
                if (StartService(TargetService, 0, NULL) == FALSE) {
                    auto err = GetLastError();
                    throw std::system_error(err, std::system_category());
                }

                StartRequested = TRUE;
            }
        }

        if (TargetServiceStatus.dwCurrentState != SERVICE_RUNNING) {
            throw std::system_error(ERROR_TIMEOUT, std::system_category());
        }
    }

    static void ServiceStop(SC_HANDLE TargetService) {
        SERVICE_STATUS TargetServiceStatus = {};
        BOOL StopRequested = FALSE;

        for (auto TimeStart = GetTickCount64(); TargetServiceStatus.dwCurrentState != SERVICE_STOPPED && GetTickCount64() - TimeStart < 3000; Sleep(1000)) {
            if (QueryServiceStatus(TargetService, &TargetServiceStatus) == FALSE) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }

            if (TargetServiceStatus.dwCurrentState == SERVICE_STOPPED) {
                break;
            }

            if (TargetServiceStatus.dwCurrentState != SERVICE_STOP_PENDING && StopRequested == FALSE) {
                if (ControlService(TargetService, SERVICE_CONTROL_STOP, &TargetServiceStatus) == FALSE) {
                    auto err = GetLastError();
                    throw std::system_error(err, std::system_category());
                }

                StopRequested = TRUE;
            }
        }

        if (TargetServiceStatus.dwCurrentState != SERVICE_STOPPED) {
            throw std::system_error(ERROR_TIMEOUT, std::system_category());
        }
    }

    void ServiceStop() {
        WSC::ResourceWrapper<WSC::ResourceTraitsServiceHandle> ScManager;
        WSC::ResourceWrapper<WSC::ResourceTraitsServiceHandle> TargetService;

        ScManager.TakeOver(OpenSCManagerW(NULL, SERVICES_ACTIVE_DATABASEW, SC_MANAGER_ALL_ACCESS));
        if (ScManager.IsValid() == false) {
            auto err = GetLastError();
            throw std::system_error(err, std::system_category());
        }

        TargetService.TakeOver(OpenServiceW(ScManager, WINDOWS_SUDO_SERVICE_NAME, SERVICE_ALL_ACCESS));
        if (ScManager.IsValid() == false) {
            auto err = GetLastError();
            throw std::system_error(err, std::system_category());
        }

        ServiceStop(TargetService);
    }

    void ServiceUninstall() {
        WSC::ResourceWrapper<WSC::ResourceTraitsServiceHandle> ScManager;
        WSC::ResourceWrapper<WSC::ResourceTraitsServiceHandle> TargetService;

        ScManager.TakeOver(OpenSCManagerW(NULL, SERVICES_ACTIVE_DATABASEW, SC_MANAGER_ALL_ACCESS));
        if (ScManager.IsValid() == false) {
            auto err = GetLastError();
            throw std::system_error(err, std::system_category());
        }

        TargetService.TakeOver(OpenServiceW(ScManager, WINDOWS_SUDO_SERVICE_NAME, SERVICE_ALL_ACCESS));
        if (ScManager.IsValid() == false) {
            auto err = GetLastError();
            throw std::system_error(err, std::system_category());
        }

        ServiceStop(TargetService);

        if (DeleteService(TargetService) == FALSE) {
            auto err = GetLastError();
            throw std::system_error(err, std::system_category());
        }
    }
}

