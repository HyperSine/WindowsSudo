#include <tchar.h>
#include <stdio.h>
#include <windows.h>
#include <stdexcept>
#include "WssService.hpp"

int Help(int code) {
    _putts(TEXT("Usage:"));
    _putts(TEXT("    WindowsSudo.Service.exe /install"));
    _putts(TEXT("    WindowsSudo.Service.exe /uninstall"));
    _putts(TEXT("    WindowsSudo.Service.exe /start"));
    _putts(TEXT("    WindowsSudo.Service.exe /stop"));
    _putts(TEXT("    WindowsSudo.Service.exe /help"));
    return code;
}

int _tmain(int argc, PTSTR argv[]) {
    WCHAR szServiceName[] = WINDOWS_SUDO_SERVICE_NAME;

    SERVICE_TABLE_ENTRYW ServiceEntry[2];
    ServiceEntry[0].lpServiceName = szServiceName;
    ServiceEntry[0].lpServiceProc = WSS::Internal::ServiceMain;
    ServiceEntry[1].lpServiceName = NULL;
    ServiceEntry[1].lpServiceProc = NULL;

    if (StartServiceCtrlDispatcherW(ServiceEntry) == FALSE) {
        try {
            if (argc == 2 && _tcsicmp(argv[1], TEXT("/install")) == 0) {
                WSS::ServiceInstall();
            } else if (argc == 2 && _tcsicmp(argv[1], TEXT("/uninstall")) == 0) {
                WSS::ServiceUninstall();
            } else if (argc == 2 && _tcsicmp(argv[1], TEXT("/start")) == 0) {
                WSS::ServiceStart();
            } else if (argc == 2 && _tcsicmp(argv[1], TEXT("/stop")) == 0) {
                WSS::ServiceStop();
            } else if (argc == 2 && (_tcsicmp(argv[1], TEXT("/help")) == 0 || _tcsicmp(argv[1], TEXT("/?")) == 0)) {
                return Help(0);
            } else {
                return Help(-1);
            }
        } catch (std::exception& e) {
            _tprintf_s(TEXT("[-] %hs\n"), e.what());
            return -1;
        }
    } else {
        return 0;
    }
}
