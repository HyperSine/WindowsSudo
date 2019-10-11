#include "WssService.hpp"
#include <sddl.h>
#include "WscResourceWrapper.hpp"
#include "WscResourceTraitsWin32.hpp"
#include "WscResourceTraitsUserEnv.hpp"
#include "WscRuntime.hpp"
#include "WssProcessCacher.hpp"

#ifdef _M_IX86
#include "WindowsSudoRpcX86.h"
#endif

#ifdef _M_X64
#include "WindowsSudoRpcX64.h"
#endif

extern "C"
DWORD WsrCreateProcessWithLogon(handle_t RpcHandle, 
                                ULONG_PTR hConsoleReference, 
                                LPCWSTR lpszUserName, 
                                LPCWSTR lpszApplicationName, 
                                LPWSTR lpszCommandLine, 
                                LPCWSTR lpszCurrentDirectory, 
                                DWORD cchEnvironment, 
                                const WCHAR lpszsEnvironment[], 
                                ULONG_PTR * phNewProcess) {
    try {
        RPC_CALL_ATTRIBUTES_V2_W CallAttributes = {};
        WSC::ResourceWrapper<WSC::ResourceTraitsGenericHandle> hClientProcess;
        WSC::ResourceWrapper<WSC::ResourceTraitsGenericHandle> hClientProcessToken;
        WSC::ResourceWrapper<WSC::ResourceTraitsHeapAlloc>  lpClientProcessTokenLoginSid;
        WSC::ResourceWrapper<WSC::ResourceTraitsGenericHandle> hAuthenticatedToken;
        WSC::ResourceWrapper<WSC::ResourceTraitsHeapAlloc> lpszUserProfileDirectory;
        WSC::ResourceWrapper<WSC::ResourceTraitsEnvironmentBlock> lpUserEnvironment;
        WSC::ResourceWrapper<WSC::ResourceTraitsGenericHandle> hNewProcess;

        *phNewProcess = 0;

        CallAttributes.Version = 2;
        CallAttributes.Flags = RPC_QUERY_CLIENT_PID;
        if (auto RpcStatus = RpcServerInqCallAttributesW(RpcHandle, &CallAttributes); RpcStatus != RPC_S_OK) {
            throw std::system_error(RpcStatus, std::system_category());
        }

        hClientProcess.TakeOver(
            OpenProcess(
                PROCESS_CREATE_PROCESS | PROCESS_DUP_HANDLE | PROCESS_QUERY_INFORMATION,
                FALSE,
                static_cast<DWORD>(
                    reinterpret_cast<ULONG_PTR>(CallAttributes.ClientPID)
                )
            )
        );
        if (hClientProcess.IsValid() == false) {
            auto err = GetLastError();
            throw std::system_error(err, std::system_category());
        }

        if (OpenProcessToken(hClientProcess, TOKEN_QUERY, hClientProcessToken.GetAddressOf()) == FALSE) {
            auto err = GetLastError();
            throw std::system_error(err, std::system_category());
        }

        lpClientProcessTokenLoginSid.TakeOver(WSC::RtlTokenGetInformation(hClientProcessToken, TokenLogonSid, NULL));
        if (lpClientProcessTokenLoginSid.As<PTOKEN_GROUPS>()->GroupCount != 1) {
            // As far as I know, a token has only one log-on SID associated.
            throw std::system_error(ERROR_ASSERTION_FAILURE, std::system_category());
        }

        WSS::Internal::HelperLaunchPrompter(
            hClientProcess,
            reinterpret_cast<HANDLE>(hConsoleReference),
            lpszUserName,
            lpClientProcessTokenLoginSid.As<PTOKEN_GROUPS>()->Groups[0].Sid,
            hAuthenticatedToken.GetAddressOf()
        );

        if (lpszCurrentDirectory == NULL) {
            DWORD cchUserProfileDirectory = 0;

            if (GetUserProfileDirectoryW(hAuthenticatedToken, NULL, &cchUserProfileDirectory) != FALSE) {
                throw std::system_error(ERROR_ASSERTION_FAILURE, std::system_category());
            }

            if (auto err = GetLastError(); err != ERROR_INSUFFICIENT_BUFFER) {
                throw std::system_error(err, std::system_category());
            }

            lpszUserProfileDirectory.TakeOver(HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, cchUserProfileDirectory * sizeof(WCHAR)));
            if (lpszUserProfileDirectory.IsValid() == false) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }

            if (GetUserProfileDirectoryW(hAuthenticatedToken, lpszUserProfileDirectory.As<PWSTR>(), &cchUserProfileDirectory) == FALSE) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }
        }

        if (lpszsEnvironment == NULL) {
            if (CreateEnvironmentBlock(lpUserEnvironment.GetAddressOf(), hAuthenticatedToken, FALSE) == FALSE) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }
        }

        WSS::Internal::HelperLaunchProcess(
            hAuthenticatedToken,
            hClientProcess,
            reinterpret_cast<HANDLE>(hConsoleReference),
            lpszApplicationName,
            lpszCommandLine,
            lpszCurrentDirectory ? lpszCurrentDirectory : lpszUserProfileDirectory.template As<PCWSTR>(),
            lpszsEnvironment ? const_cast<PWSTR>(lpszsEnvironment) : lpUserEnvironment.Get(),
            hNewProcess.GetAddressOf(),
            NULL
        );

        if (DuplicateHandle(GetCurrentProcess(), hNewProcess.Transfer(), 
                            hClientProcess, hNewProcess.GetAddressOf(), 
                            SYNCHRONIZE | PROCESS_QUERY_LIMITED_INFORMATION, 
                            FALSE, 
                            DUPLICATE_CLOSE_SOURCE) == FALSE) 
        {
            auto err = GetLastError();
            throw std::system_error(err, std::system_category());
        }

        *phNewProcess = hNewProcess.TransferAs<ULONG_PTR>();

        return ERROR_SUCCESS;
    } catch (std::system_error& e) {
        return e.code().value();
    }
}

extern "C"
DWORD WsrCreateProcessAsElevated(handle_t RpcHandle, 
                                 ULONG_PTR hConsoleReference, 
                                 LPCWSTR lpszApplicationName, 
                                 LPWSTR lpszCommandLine, 
                                 LPCWSTR lpszCurrentDirectory, 
                                 DWORD cchEnvironment, 
                                 const WCHAR lpszsEnvironment[], 
                                 ULONG_PTR *phNewProcess) {
    try {
        RPC_CALL_ATTRIBUTES_V2_W CallAttributes = {};

        WSC::ResourceWrapper<WSC::ResourceTraitsGenericHandle> hClientProcess;
        WSC::ResourceWrapper<WSC::ResourceTraitsGenericHandle> hClientParentProcess;
        WSC::ResourceWrapper<WSC::ResourceTraitsGenericHandle> hClientParentProcessToken;
        WSC::ResourceWrapper<WSC::ResourceTraitsHeapAlloc> lpClientParentProcessTokenUser;
        WSC::ResourceWrapper<WSC::ResourceTraitsHeapAlloc> lpszClientParentProcessUserName;
        WSC::ResourceWrapper<WSC::ResourceTraitsHeapAlloc> lpszClientParentProcessDomainName;
        WSC::ResourceWrapper<WSC::ResourceTraitsGenericHandle> hNewProcess;

        *phNewProcess = 0;

        CallAttributes.Version = 2;
        CallAttributes.Flags = RPC_QUERY_CLIENT_PID;
        if (auto RpcStatus = RpcServerInqCallAttributesW(RpcHandle, &CallAttributes); RpcStatus != RPC_S_OK) {
            throw std::system_error(RpcStatus, std::system_category());
        }

        hClientProcess.TakeOver(
            OpenProcess(
                PROCESS_CREATE_PROCESS | PROCESS_DUP_HANDLE | PROCESS_QUERY_INFORMATION,
                FALSE,
                static_cast<DWORD>(
                    reinterpret_cast<ULONG_PTR>(CallAttributes.ClientPID)
                )
            )
        );
        if (hClientProcess.IsValid() == false) {
            auto err = GetLastError();
            throw std::system_error(err, std::system_category());
        }

        hClientParentProcess.TakeOver(
            OpenProcess(
                PROCESS_QUERY_INFORMATION | PROCESS_QUERY_LIMITED_INFORMATION,
                FALSE,
                WSC::RtlProcessGetParentProcessId(hClientProcess)
            )
        );
        if (hClientProcess.IsValid() == false) {
            auto err = GetLastError();
            throw std::system_error(err, std::system_category());
        }

        if (OpenProcessToken(hClientParentProcess, TOKEN_ALL_ACCESS, hClientParentProcessToken.GetAddressOf()) == FALSE) {
            auto err = GetLastError();
            throw std::system_error(err, std::system_category());
        }

        lpClientParentProcessTokenUser.TakeOver(
            WSC::RtlTokenGetInformation(hClientParentProcessToken, TokenUser, NULL)
        );

        {
            DWORD cchUserName = 0;
            DWORD cchDomainName = 0;
            SID_NAME_USE NameUse;
            
            if (LookupAccountSidW(NULL, lpClientParentProcessTokenUser.As<PTOKEN_USER>()->User.Sid, NULL, &cchUserName, NULL, &cchDomainName, &NameUse) != FALSE) {
                throw std::system_error(ERROR_ASSERTION_FAILURE, std::system_category());
            }

            lpszClientParentProcessUserName.TakeOver(HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, cchUserName * sizeof(WCHAR)));
            if (lpszClientParentProcessUserName.IsValid() == false) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }

            lpszClientParentProcessDomainName.TakeOver(HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, cchDomainName * sizeof(WCHAR)));
            if (lpszClientParentProcessDomainName.IsValid() == false) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }

            if (LookupAccountSidW(NULL, lpClientParentProcessTokenUser.As<PTOKEN_USER>()->User.Sid, lpszClientParentProcessUserName.As<PWSTR>(), &cchUserName, lpszClientParentProcessDomainName.As<PWSTR>(), &cchDomainName, &NameUse) == FALSE) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }
        }

        if (WSC::RtlTokenIsElevated(hClientParentProcessToken) == FALSE) {
            TOKEN_LINKED_TOKEN LinkedToken;

            if (DWORD _; GetTokenInformation(hClientParentProcessToken, TokenLinkedToken, &LinkedToken, sizeof(TOKEN_LINKED_TOKEN), &_) == FALSE) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }

            hClientParentProcessToken.TakeOver(LinkedToken.LinkedToken);

            if (WSS::Internal::ProcessCacher::GetInstance().Query(GetProcessId(hClientParentProcess)) == false) {
                WSC::ResourceWrapper<WSC::ResourceTraitsGenericHandle> hToken;

                WSS::Internal::HelperLaunchPrompter(
                    hClientProcess,
                    reinterpret_cast<HANDLE>(hConsoleReference),
                    lpszClientParentProcessUserName.As<PCWSTR>(),
                    NULL,
                    hToken.GetAddressOf()
                );

                WSS::Internal::ProcessCacher::GetInstance().Insert(GetProcessId(hClientParentProcess), hClientParentProcess);

                hClientParentProcess.Discard();
            }
        }

        WSS::Internal::HelperLaunchProcess(
            hClientParentProcessToken,
            hClientProcess,
            reinterpret_cast<HANDLE>(hConsoleReference),
            lpszApplicationName,
            lpszCommandLine,
            lpszCurrentDirectory,
            const_cast<PWSTR>(lpszsEnvironment),
            hNewProcess.GetAddressOf(),
            NULL
        );

        if (DuplicateHandle(GetCurrentProcess(), hNewProcess.Transfer(),
                            hClientProcess, hNewProcess.GetAddressOf(),
                            SYNCHRONIZE | PROCESS_QUERY_LIMITED_INFORMATION,
                            FALSE,
                            DUPLICATE_CLOSE_SOURCE) == FALSE) 
        {
            auto err = GetLastError();
            throw std::system_error(err, std::system_category());
        }

        *phNewProcess = hNewProcess.TransferAs<ULONG_PTR>();

        return ERROR_SUCCESS;
    } catch (std::system_error& e) {
        return e.code().value();
    }
}

