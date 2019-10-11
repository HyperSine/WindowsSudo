#include <tchar.h>
#include <stdio.h>
#include <windows.h>
#include <winternl.h>
#include <locale.h>
#include <optional>
#include "WscResourceWrapper.hpp"
#include "WscResourceTraitsWin32.hpp"
#include "WscResourceTraitsWinRpc.hpp"
#include "Xstring.hpp"

#ifdef _M_IX86
#include "WindowsSudoRpcX86.h"
#endif

#ifdef _M_X64
#include "WindowsSudoRpcX64.h"
#endif

#pragma comment(lib, "rpcrt4")

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

DWORD SuMain(RPC_BINDING_HANDLE RpcHandle, BOOL bLoginShell, BOOL bPreserveEnvironment, LPCWSTR lpszUserName, LPCWSTR lpszShellPath, LPCWSTR lpszCommand, PHANDLE lpNewProcessHandle) {
    static HANDLE(*lpfnBaseGetConsoleReference)() = reinterpret_cast<HANDLE(*)()>(
        GetProcAddress(GetModuleHandle(TEXT("kernelbase.dll")), "BaseGetConsoleReference")
    );

    DWORD Win32Status;

    DWORD cchCommandLine = 0;
    PWSTR lpszCommandLine = NULL;

    DWORD cchCurrentDirectory = 0;
    PWSTR lpszCurrentDirectory = NULL;

    DWORD cchEnvironment = 0;
    PWSTR lpszsEnvironment = NULL;

    ULONG_PTR NewProcessHandle = 0;

    *lpNewProcessHandle = NULL;

    if (lpszShellPath == NULL) {
        lpszShellPath = L"cmd.exe";
    }

    if (lpszCommand) {
        lpszCommand = L"";
    }

    cchCommandLine = _scwprintf(L"\"%s\" %s", lpszShellPath, lpszCommand) + 1;

    lpszCommandLine = reinterpret_cast<PWSTR>(HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, cchCommandLine * sizeof(WCHAR)));
    if (lpszCommandLine == NULL) {
        Win32Status = GetLastError();
        goto ON_FAILURE;
    }

    swprintf_s(lpszCommandLine, cchCommandLine, L"\"%s\" %s", lpszShellPath, lpszCommand);

    if (bLoginShell == FALSE) {
        cchCurrentDirectory = GetCurrentDirectoryW(0, NULL);
        if (cchCurrentDirectory == 0) {
            Win32Status = GetLastError();
            goto ON_FAILURE;
        }

        lpszCurrentDirectory = reinterpret_cast<PWSTR>(HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, cchCurrentDirectory * sizeof(WCHAR)));
        if (lpszCurrentDirectory == NULL) {
            Win32Status = GetLastError();
            goto ON_FAILURE;
        }

        if (GetCurrentDirectoryW(cchCurrentDirectory, lpszCurrentDirectory) == 0) {
            Win32Status = GetLastError();
            goto ON_FAILURE;
        }
    }

    if (bPreserveEnvironment) {
        lpszsEnvironment = GetEnvironmentStringsW();
        if (lpszsEnvironment == NULL) {
            Win32Status = GetLastError();
            goto ON_FAILURE;
        }

        for (cchEnvironment = 0; lpszsEnvironment[cchEnvironment]; ++cchEnvironment) {
            cchEnvironment += static_cast<DWORD>(wcslen(lpszsEnvironment + cchEnvironment));
        }

        ++cchEnvironment;
    }

    RpcTryExcept
        Win32Status = WsrCreateProcessWithLogon(
            RpcHandle,
            reinterpret_cast<ULONG_PTR>(lpfnBaseGetConsoleReference()),
            lpszUserName,
            NULL,
            lpszCommandLine,
            lpszCurrentDirectory,
            cchEnvironment,
            lpszsEnvironment,
            &NewProcessHandle
        );
    RpcExcept(EXCEPTION_EXECUTE_HANDLER)
        Win32Status = RpcExceptionCode();
    RpcEndExcept

    if (Win32Status != ERROR_SUCCESS) {
        goto ON_FAILURE;
    }

    *lpNewProcessHandle = reinterpret_cast<HANDLE>(NewProcessHandle);

ON_FAILURE:

    if (lpszsEnvironment) {
        FreeEnvironmentStringsW(lpszsEnvironment);
        lpszsEnvironment = NULL;
    }

    if (lpszCurrentDirectory) {
        HeapFree(GetProcessHeap(), 0, lpszCurrentDirectory);
        lpszCurrentDirectory = NULL;
    }

    if (lpszCommandLine) {
        HeapFree(GetProcessHeap(), 0, lpszCommandLine);
        lpszCommandLine = NULL;
    }

    return Win32Status;
}

int Help(int code) {
    _putts(TEXT("Usage: su [options] <LOGIN>"));
    _putts(TEXT(""));
    _putts(TEXT("Options:"));
    _putts(TEXT("  -c, --command COMMAND        pass COMMAND to the invoked shell"));
    _putts(TEXT("  -h, --help                   display this help message and exit"));
    _putts(TEXT("  -, -l, --login               make the shell a login shell"));
    _putts(TEXT("  -m, -p,"));
    _putts(TEXT("  --preserve-environment       do not reset environment variables, and"));
    _putts(TEXT("                               keep the same shell"));
    _putts(TEXT("  -s, --shell SHELL            use SHELL instead of %ComSpec%"));
    _putts(TEXT(""));
    return code;
}

int _tmain(int argc, PTSTR argv[]) {
    setlocale(LC_ALL, "");
    SetConsoleCtrlHandler(NULL, TRUE);  // Disable Ctrl+C

    std::optional<bool> bShowHelp = std::nullopt;
    std::optional<bool> bLoginShell = std::nullopt;
    std::optional<bool> bPreserveEnvironment = std::nullopt;
    std::optional<std::Xstring> szCommand = std::nullopt;
    std::optional<std::Xstring> szShellPath = std::nullopt;
    std::optional<std::Xstring> szUsername = std::nullopt;

    for (int i = 1; i < argc; ++i) {
        if (_tcscmp(argv[i], TEXT("-c")) == 0 || _tcscmp(argv[i], TEXT("--command")) == 0) {
            ++i;
            if (i < argc) {
                if (szCommand.has_value() == false) {
                    szCommand.emplace(std::Xstring(argv[i]));
                } else {
                    _tprintf_s(TEXT("su: option '%s' is set with multiple times\n"), argv[i - 1]);
                    return Help(-1);
                }
            } else {
                _tprintf_s(TEXT("su: option '%s' requires an argument\n"), argv[i - 1]);
                return Help(-1);
            }
        } else if (_tcscmp(argv[i], TEXT("-h")) == 0 || _tcscmp(argv[i], TEXT("--help")) == 0) {
            if (bShowHelp.has_value() == false) {
                bShowHelp.emplace(true);
            } else {
                _tprintf_s(TEXT("su: option '%s' is set with multiple times\n"), argv[i]);
                return Help(-1);
            }
        } else if (_tcscmp(argv[i], TEXT("-")) == 0 || _tcscmp(argv[i], TEXT("-l")) == 0 || _tcscmp(argv[i], TEXT("--login")) == 0) {
            if (bLoginShell.has_value() == false) {
                bLoginShell.emplace(true);
            } else {
                _tprintf_s(TEXT("su: option '%s' is set with multiple times\n"), argv[i]);
                return Help(-1);
            }
        } else if (_tcscmp(argv[i], TEXT("-m")) == 0 || _tcscmp(argv[i], TEXT("-p")) == 0 || _tcscmp(argv[i], TEXT("--preserve-environment")) == 0) {
            if (bPreserveEnvironment.has_value() == false) {
                bPreserveEnvironment.emplace(true);
            } else {
                _tprintf_s(TEXT("su: option '%s' is set with multiple times\n"), argv[i]);
                return Help(-1);
            }
        } else if (_tcscmp(argv[i], TEXT("-s")) == 0 || _tcscmp(argv[i], TEXT("--shell")) == 0) {
            ++i;
            if (i < argc) {
                if (szShellPath.has_value() == false) {
                    szShellPath.emplace(std::Xstring(argv[i]));
                } else {
                    _tprintf_s(TEXT("su: option '%s' is set with multiple times\n"), argv[i - 1]);
                    return Help(-1);
                }
            } else {
                _tprintf_s(TEXT("su: option '%s' requires an argument\n"), argv[i - 1]);
                return Help(-1);
            }
        } else {
            if (i + 1 == argc) {
                szUsername.emplace(std::Xstring(argv[i]));
            } else {
                _tprintf_s(TEXT("su: unrecognized option '%s'\n"), argv[i]);
            }
        }
    }

    if (szUsername.has_value() == false || szUsername->empty()) {
        _tprintf_s(TEXT("su: username is not specified\n"));
        return Help(-1);
    }

    if (bShowHelp.value_or(false)) {
        return Help(0);
    }

    try {
        RPC_STATUS RpcStatus;
        DWORD Win32Status;
        RPC_WSTR lpszProtocolSequence = reinterpret_cast<RPC_WSTR>(const_cast<PWSTR>(L"ncalrpc"));
        RPC_WSTR lpszEndpoint = reinterpret_cast<RPC_WSTR>(const_cast<PWSTR>(L"WindowsSudoRpc"));
        WSC::ResourceWrapper<WSC::ResourceTraitsRpcStringW> lpszStringBinding;
        WSC::ResourceWrapper<WSC::ResourceTraitsRpcBindHandle> RpcHandle;
        WSC::ResourceWrapper<WSC::ResourceTraitsGenericHandle> NewProcessHandle;

        RpcStatus = RpcStringBindingComposeW(NULL, lpszProtocolSequence, NULL, lpszEndpoint, NULL, lpszStringBinding.GetAddressOf());
        if (RpcStatus != RPC_S_OK) {
            throw std::system_error(RpcStatus, std::system_category());
        }

        RpcStatus = RpcBindingFromStringBindingW(lpszStringBinding, RpcHandle.GetAddressOf());
        if (RpcStatus != RPC_S_OK) {
            throw std::system_error(RpcStatus, std::system_category());
        }

        Win32Status = SuMain(
            RpcHandle,
            bLoginShell.value_or(false) ? TRUE : FALSE,
            bPreserveEnvironment.value_or(false) ? TRUE : FALSE,
            szUsername->explicit_wstring().c_str(),
            szShellPath.has_value() ? szShellPath->explicit_wstring().c_str() : NULL,
            szCommand.has_value() ? szCommand->explicit_wstring().c_str() : NULL,
            NewProcessHandle.GetAddressOf()
        );

        if (Win32Status == ERROR_SUCCESS) {
            WaitForSingleObject(NewProcessHandle, INFINITE);

            if (GetExitCodeProcess(NewProcessHandle, &Win32Status) == FALSE) {
                Win32Status = GetLastError();
                throw std::system_error(Win32Status, std::system_category());
            }

            return Win32Status;
        } else if (Win32Status == ERROR_CANCELLED) {
            return Win32Status;
        } else {
            throw std::system_error(Win32Status, std::system_category());
        }
    } catch (std::system_error & e) {
        _tprintf_s(TEXT("su: [0x%.8x] %hs\n"), e.code().value(), e.what());
        return e.code().value();
    }
}

