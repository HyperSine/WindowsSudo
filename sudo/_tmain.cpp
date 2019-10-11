#include <tchar.h>
#include <stdio.h>
#include <windows.h>
#include <locale.h>
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

DWORD SudoMain(RPC_BINDING_HANDLE RpcHandle, PWSTR lpszCommandLine, PHANDLE phNewProcess) {
    static HANDLE(*lpfnBaseGetConsoleReference)() = reinterpret_cast<HANDLE(*)()>(
        GetProcAddress(GetModuleHandle(TEXT("kernelbase.dll")), "BaseGetConsoleReference")
    );

    DWORD Win32Status;

    DWORD cchCurrentDirectory = 0;
    PWSTR lpszCurrentDirectory = NULL;

    DWORD cchEnvironment = 0;
    PWSTR lpszsEnvironment = NULL;

    ULONG_PTR NewProcessHandle = 0;

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

    lpszsEnvironment = GetEnvironmentStringsW();
    if (lpszsEnvironment == NULL) {
        Win32Status = GetLastError();
        goto ON_FAILURE;
    }

    for (cchEnvironment = 0; lpszsEnvironment[cchEnvironment]; ++cchEnvironment) {
        cchEnvironment += static_cast<DWORD>(wcslen(lpszsEnvironment + cchEnvironment));
    }

    ++cchEnvironment;

    RpcTryExcept
        Win32Status = WsrCreateProcessAsElevated(
            RpcHandle,
            reinterpret_cast<ULONG_PTR>(lpfnBaseGetConsoleReference()),
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

    *phNewProcess = reinterpret_cast<HANDLE>(NewProcessHandle);

ON_FAILURE:

    if (lpszsEnvironment) {
        FreeEnvironmentStringsW(lpszsEnvironment);
        lpszsEnvironment = NULL;
    }

    if (lpszCurrentDirectory) {
        HeapFree(GetProcessHeap(), 0, lpszCurrentDirectory);
        lpszCurrentDirectory = NULL;
    }

    return Win32Status;
}

int Help(int code) {
    _putts(TEXT("Usage: sudo ..."));
    _putts(TEXT(""));
    return code;
}

int _tmain(int argc, PTSTR argv[]) {
    setlocale(LC_ALL, "");
    SetConsoleCtrlHandler(NULL, TRUE);  // Disable Ctrl+C

    try {
        if (argc == 1) {
            return Help(-1);
        }

        DWORD Win32Status;
        RPC_STATUS RpcStatus;
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

        std::Xstring szCommandLine;
        for (int i = 1; i < argc; ++i) {
            szCommandLine.append(1, TEXT('"'));
            szCommandLine.append(argv[i]);
            szCommandLine.append(1, TEXT('"'));
            szCommandLine.append(1, TEXT(' '));
        }

        Win32Status = SudoMain(
            RpcHandle,
            szCommandLine.explicit_wstring().data(),
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
    } catch (std::system_error& e) {
        _tprintf_s(TEXT("sudo: [0x%.8x] %hs\n"), e.code().value(), e.what());
        return e.code().value();
    }
}

