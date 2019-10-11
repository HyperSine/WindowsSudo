#pragma once
#include <windows.h>

#define WINDOWS_SUDO_SERVICE_NAME L"WindowsSudoSvc"
#define WINDOWS_SUDO_SERVICE_DISPLAYNAME L"Windows Sudo Service"

namespace WSS {

    void ServiceInstall();
    void ServiceStart();
    void ServiceStop();
    void ServiceUninstall();

    namespace Internal {

        inline HANDLE g_hLsassProcessToken;

        DWORD ServiceSetStatus(
            _In_ DWORD dwCurrentState,
            _In_ DWORD dwControlsAccepted,
            _In_ DWORD dwWin32ExitCode
        );

        VOID WINAPI ServiceMain(
            _In_ DWORD dwNumServicesArgs,
            _In_ PTSTR lpServiceArgVectors[]
        );

        [[nodiscard]]
        VOID HelperLaunchProcess(
            _In_ HANDLE hToken,
            _In_ HANDLE hParentProcess,
            _In_ HANDLE hConsoleReference,
            _In_opt_ PCWSTR lpszApplicationName,
            _Inout_opt_ PWSTR lpszCommandLine,
            _In_opt_ PCWSTR lpszCurrentDirectory,
            _In_opt_ PVOID lpEnvironment,
            _Out_opt_ PHANDLE phProcess,
            _Out_opt_ PHANDLE phThread
        );

        [[nodiscard]]
        VOID HelperLaunchPrompter(
            _In_ HANDLE hClientProcess,
            _In_ HANDLE hClientConsoleReference,
            _In_ PCWSTR lpszUserName,
            _In_opt_ PSID lpLogonSid,
            _Out_ PHANDLE phAuthenticatedToken
        );
    }

}

