#define UMDF_USING_NTSTATUS
#include <ntstatus.h>
#include "WssService.hpp"
#include <winternl.h>
#include "WscResourceWrapper.hpp"
#include "WscResourceTraitsWin32.hpp"
#include "WscRuntime.hpp"
#include "Xstring.hpp"
#include <sddl.h>
#include <system_error>

#pragma comment(lib, "ntdll")

#define ProcThreadAttributeConsoleReference ((PROC_THREAD_ATTRIBUTE_NUM)10)
#define PROC_THREAD_ATTRIBUTE_CONSOLE_REFERENCE \
    ProcThreadAttributeValue(ProcThreadAttributeConsoleReference, FALSE, TRUE, FALSE)

namespace WSS::Internal {

    [[nodiscard]]
    VOID HelperLaunchProcess(_In_ HANDLE hToken,
                             _In_ HANDLE hParentProcess,
                             _In_ HANDLE hConsoleReference,
                             _In_opt_ PCWSTR lpszApplicationName,
                             _Inout_opt_ PWSTR lpszCommandLine,
                             _In_opt_ PCWSTR lpszCurrentDirectory,
                             _In_opt_ PVOID lpEnvironment,
                             _Out_opt_ PHANDLE phProcess,
                             _Out_opt_ PHANDLE phThread) {
        SIZE_T cbBuffer = 0;
        WSC::ResourceWrapper<WSC::ResourceTraitsHeapAlloc> lpBuffer;

        STARTUPINFOEXW StartupInfoEx = { sizeof(STARTUPINFOEXW) };
        PROCESS_INFORMATION ProcessInfo = {};

        if (phProcess) {
            *phProcess = NULL;
        }

        if (phThread) {
            *phThread = NULL;
        }

        if (InitializeProcThreadAttributeList(NULL, 2, 0, &cbBuffer) != FALSE) {
            throw std::system_error(ERROR_ASSERTION_FAILURE, std::system_category());
        }

        if (auto err = GetLastError(); err != ERROR_INSUFFICIENT_BUFFER) {
            throw std::system_error(err, std::system_category());
        }

        lpBuffer.TakeOver(HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, cbBuffer));
        if (lpBuffer.IsValid() == false) {
            auto err = GetLastError();
            throw std::system_error(err, std::system_category());
        }

        StartupInfoEx.lpAttributeList = lpBuffer.As<LPPROC_THREAD_ATTRIBUTE_LIST>();
        __analysis_assume(StartupInfoEx.lpAttributeList != NULL);

        if (InitializeProcThreadAttributeList(StartupInfoEx.lpAttributeList, 2, 0, &cbBuffer) == FALSE) {
            auto err = GetLastError();
            throw std::system_error(err, std::system_category());
        }

        if (UpdateProcThreadAttribute(StartupInfoEx.lpAttributeList, 0, PROC_THREAD_ATTRIBUTE_PARENT_PROCESS, &hParentProcess, sizeof(HANDLE), NULL, NULL) == FALSE) {
            auto err = GetLastError();
            throw std::system_error(err, std::system_category());
        }

        if (UpdateProcThreadAttribute(StartupInfoEx.lpAttributeList, 0, PROC_THREAD_ATTRIBUTE_CONSOLE_REFERENCE, &hConsoleReference, sizeof(HANDLE), NULL, NULL) == FALSE) {
            auto err = GetLastError();
            throw std::system_error(err, std::system_category());
        }

        if (CreateProcessAsUserW(hToken, 
                                 lpszApplicationName, 
                                 lpszCommandLine, 
                                 NULL, 
                                 NULL, 
                                 TRUE,      // must be TRUE, otherwise hConsoleReference cannot be inherited.
                                 EXTENDED_STARTUPINFO_PRESENT | (lpEnvironment ? CREATE_UNICODE_ENVIRONMENT : 0), 
                                 lpEnvironment, 
                                 lpszCurrentDirectory, 
                                 &StartupInfoEx.StartupInfo, 
                                 &ProcessInfo) == FALSE) {
            auto err = GetLastError();
            throw std::system_error(err, std::system_category());
        } else {
            if (phProcess) {
                *phProcess = ProcessInfo.hProcess;
            } else {
                CloseHandle(ProcessInfo.hProcess);
            }

            if (phThread) {
                *phThread = ProcessInfo.hThread;
            } else {
                CloseHandle(ProcessInfo.hThread);
            }
        }
    }

    [[nodiscard]]
    VOID HelperLaunchPrompter(_In_ HANDLE hClientProcess,
                              _In_ HANDLE hClientConsoleReference,
                              _In_ PCWSTR lpszUserName,
                              _In_opt_ PSID lpLogonSid,
                              _Out_ PHANDLE phAuthenticatedToken) {
        WSC::ResourceWrapper<WSC::ResourceTraitsGenericHandle> hPrompterToken;
        WSC::ResourceWrapper<WSC::ResourceTraitsLocalAlloc> lpszLogonSid;
        std::Xstring szCommandLine;
        WSC::ResourceWrapper<WSC::ResourceTraitsGenericHandle> hPrompterProcess;
        HANDLE hAuthenticatedToken;

        *phAuthenticatedToken = NULL;

        //
        // Prepare hPrompterToken
        //
        {
            DWORD dwPrompterSessionId;
            if (ProcessIdToSessionId(GetProcessId(hClientProcess), &dwPrompterSessionId) == FALSE) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }

            if (DuplicateTokenEx(g_hLsassProcessToken, TOKEN_ALL_ACCESS, NULL, SecurityIdentification, TokenPrimary, hPrompterToken.GetAddressOf()) == FALSE) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }

            if (SetTokenInformation(hPrompterToken, TokenSessionId, &dwPrompterSessionId, sizeof(DWORD)) == FALSE) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }
        }

        //
        // Prepare lpszLogonSid
        //
        if (lpLogonSid) {
            if (ConvertSidToStringSidW(lpLogonSid, lpszLogonSid.GetAddressOfAs<PWSTR*>()) == FALSE) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }

            szCommandLine = std::Xstring::format(
                TEXT("WindowsSudo.Prompter.exe %u \"%ws\" \"%ws\""),
                GetCurrentProcessId(),
                lpszUserName,
                lpszLogonSid.As<PCWSTR>()
            );
        } else {
            szCommandLine = std::Xstring::format(
                TEXT("WindowsSudo.Prompter.exe %u \"%ws\""),
                GetCurrentProcessId(),
                lpszUserName
            );
        }

        //
        // Launch prompter and wait it return
        //
        HelperLaunchProcess(
            hPrompterToken,
            hClientProcess,
            hClientConsoleReference,
            TEXT("WindowsSudo.Prompter.exe"),
            szCommandLine.explicit_wstring().data(),
            NULL,
            NULL,
            hPrompterProcess.GetAddressOf(),
            NULL
        );

        WaitForSingleObject(hPrompterProcess, INFINITE);

        //
        // Get exit code. 
        // 1. Return a handle of new token if positive
        // 2. Return error code if negative
        //
        {
            DWORD dwProcessExitCode;
            if (GetExitCodeProcess(hPrompterProcess, &dwProcessExitCode) == FALSE) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }

            if (static_cast<int>(dwProcessExitCode) < 0) {
                throw std::system_error(-static_cast<int>(dwProcessExitCode), std::system_category());
            }

            hAuthenticatedToken = reinterpret_cast<HANDLE>(static_cast<ULONG_PTR>(dwProcessExitCode));
        }

        if (WSC::RtlObjectValidateType(hAuthenticatedToken, L"Token") == FALSE) {
            throw std::system_error(ERROR_INVALID_HANDLE, std::system_category());
        }

        *phAuthenticatedToken = hAuthenticatedToken;
    }
}

