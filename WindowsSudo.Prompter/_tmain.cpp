#include <tchar.h>
#include <stdio.h>
#include <locale.h>
#include <windows.h>
#include <wincred.h>
#include <sddl.h>
#include "WscResourceWrapper.hpp"
#include "WscResourceTraitsWin32.hpp"
#include "WscRuntime.hpp"
#include "WspRuntime.hpp"
#include <system_error>

#pragma comment(lib, "credui")

TCHAR g_szTargetName[CREDUI_MAX_GENERIC_TARGET_LENGTH + 1] = {};
TCHAR g_szDomainName[CREDUI_MAX_DOMAIN_TARGET_LENGTH + 1] = {};
TCHAR g_szUserName[CREDUI_MAX_USERNAME_LENGTH + 1] = {};
TCHAR g_szPassword[CREDUI_MAX_PASSWORD_LENGTH + 1] = {};

int PrompterMain(DWORD dwServiceProcessId, PCTSTR lpszUserName, PSID lpLogonSid) {
    WSC::ResourceWrapper<WSC::ResourceTraitsGenericHandle> hWssProcess;
    WSC::ResourceWrapper<WSC::ResourceTraitsGenericHandle> hNewToken;

    //
    // Open WindowsSudo.Service, prepare to pass authenticated token
    //
    hWssProcess.TakeOver(OpenProcess(PROCESS_DUP_HANDLE, FALSE, dwServiceProcessId));
    if (hWssProcess.IsValid() == false) {
        auto err = GetLastError();
        throw std::system_error(err, std::system_category());
    }

    for (size_t attempt = 0; attempt < 3; ++attempt, Sleep(1000)) {
        DWORD CreduiStatus;

        _tcscpy_s(g_szTargetName, lpszUserName);

        //
        // Prompt for password
        //
        CreduiStatus = CredUICmdLinePromptForCredentials(
            g_szTargetName,
            NULL,
            0,
            g_szTargetName, RTL_NUMBER_OF(g_szTargetName),
            g_szPassword, RTL_NUMBER_OF(g_szPassword),
            NULL,
            CREDUI_FLAGS_DO_NOT_PERSIST | CREDUI_FLAGS_EXCLUDE_CERTIFICATES | CREDUI_FLAGS_USERNAME_TARGET_CREDENTIALS | CREDUI_FLAGS_VALIDATE_USERNAME
        );
        if (CreduiStatus != NO_ERROR) {
            throw std::system_error(CreduiStatus, std::system_category());
        }

        //
        // Parsing TargetName to UserName and Password
        //
        CreduiStatus = CredUIParseUserName(
            g_szTargetName,
            g_szUserName, RTL_NUMBER_OF(g_szUserName),
            g_szDomainName, RTL_NUMBER_OF(g_szDomainName)
        );
        if (CreduiStatus != NO_ERROR) {
            throw std::system_error(CreduiStatus, std::system_category());
        }

        if (LogonUser(g_szUserName,
                      g_szDomainName,
                      g_szPassword,
                      lpLogonSid ? LOGON32_LOGON_INTERACTIVE : LOGON32_LOGON_NETWORK,
                      LOGON32_PROVIDER_DEFAULT,
                      hNewToken.GetAddressOf())) 
        {
            SecureZeroMemory(g_szPassword, sizeof(g_szPassword));

            if (lpLogonSid) {
                if (HANDLE hToken; WSC::RtlTokenElevateIfPossible(hNewToken, &hToken)) {
                    hNewToken.Release();
                    hNewToken.TakeOver(hToken);
                }

                hNewToken.TakeOver(
                    WSP::RtlTokenCreateForLogonSid(hNewToken, lpLogonSid)
                );
            }

            if (DuplicateHandle(GetCurrentProcess(), hNewToken.Transfer(), hWssProcess, hNewToken.GetAddressOf(), 0, FALSE, DUPLICATE_CLOSE_SOURCE | DUPLICATE_SAME_ACCESS) == FALSE) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }

            return static_cast<int>(hNewToken.TransferAs<LONG_PTR>());
        } else {
            SecureZeroMemory(g_szPassword, sizeof(g_szPassword));

            if (auto err = GetLastError(); err == ERROR_LOGON_FAILURE) {
                _putts(TEXT("Authentication failure!"));
            } else {
                throw std::system_error(err, std::system_category());
            }
        }
    }

    throw std::system_error(ERROR_ACCESS_DENIED, std::system_category());
}

//
// WindowsSudo.Prompter.exe <PID of WindowsSudo.Service.exe> <UserName to prompt> [Requester log-on SID]
//
int _tmain(int argc, PTSTR argv[]) {
    setlocale(LC_ALL, "");

    try {
        DWORD dwServiceProcessId;
        WSC::ResourceWrapper<WSC::ResourceTraitsLocalAlloc> lpLogonSid;

        if (SetConsoleCtrlHandler([](DWORD dwCtrlType) -> BOOL { ExitProcess(-ERROR_CANCELLED); return TRUE; }, TRUE) == FALSE) {
            auto err = GetLastError();
            throw std::system_error(err, std::system_category());
        }

        //
        // Make sure prompter is run as 'NT Authority\System' and has SeCreateTokenPrivilege enabled.
        //
        if (WSC::RtlIsLocalSystem()) {
            WSC::ResourceWrapper<WSC::ResourceTraitsGenericHandle> hCurrentProcessToken;

            if (OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, hCurrentProcessToken.GetAddressOf()) == FALSE) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }

            if (BOOL bEnabled; WSC::RtlTokenCheckPrivilege(hCurrentProcessToken, SE_CREATE_TOKEN_NAME, &bEnabled) == FALSE) {
                throw std::system_error(ERROR_ACCESS_DENIED, std::system_category());
            } else {
                if (bEnabled == FALSE) {
                    WSC::RtlTokenModifyPrivilege(hCurrentProcessToken, SE_CREATE_TOKEN_NAME, TRUE);
                }
            }
        } else {
            throw std::system_error(ERROR_ACCESS_DENIED, std::system_category());
        }

        if (3 <= argc && argc <= 4) {
            if (_stscanf_s(argv[1], TEXT("%u"), &dwServiceProcessId) != 1) {
                throw std::system_error(ERROR_INVALID_PARAMETER, std::system_category());
            }

            if (_tcslen(argv[2]) > CREDUI_MAX_USERNAME_LENGTH) {
                throw std::system_error(ERROR_INVALID_PARAMETER, std::system_category());
            }

            //
            // Serialize target log-on SID, if have
            //
            if (argc == 4 && ConvertStringSidToSid(argv[3], lpLogonSid.GetAddressOfAs<PSID*>()) == FALSE) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }
        } else {
            throw std::system_error(ERROR_INVALID_PARAMETER, std::system_category());
        }

        return PrompterMain(dwServiceProcessId, argv[2], lpLogonSid);
    } catch (std::system_error& e) {
        return -e.code().value();
    }
}

