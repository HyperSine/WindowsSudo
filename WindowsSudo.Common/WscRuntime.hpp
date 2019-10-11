#pragma once
#include <windows.h>

namespace WSC {

    [[nodiscard]]
    BOOL RtlIsLocalSystem();

    [[nodiscard]]
    BOOL RtlObjectValidateType(
        _In_ HANDLE hObject, 
        _In_ PCWSTR lpszObjectName
    );

    [[nodiscard]]
    PVOID RtlTokenGetInformation(
        _In_ HANDLE hToken, 
        _In_ TOKEN_INFORMATION_CLASS TokenInformationClass, 
        _Out_opt_ PDWORD pcbInformation
    );

    VOID RtlTokenSetInformation(
        _In_ HANDLE hToken, 
        _In_ TOKEN_INFORMATION_CLASS TokenInformationClass, 
        _In_ PVOID lpInformation, 
        _In_ DWORD cbInformation
    );

    [[nodiscard]]
    BOOL RtlTokenIsElevated(
        _In_ HANDLE hToken
    );

    [[nodiscard]]
    BOOL RtlTokenCheckPrivilege(
        _In_ HANDLE hToken, 
        _In_ PCTSTR lpszPrivilegeName, 
        _Out_opt_ PBOOL pbEnable
    );

    [[nodiscard]]
    BOOL RtlTokenElevateIfPossible(
        _In_ HANDLE hToken, 
        _Out_ PHANDLE phNewToken
    );

    VOID RtlTokenModifyPrivilege(
        _In_ HANDLE hToken, 
        _In_ PCTSTR lpszPrivilegeName,
        _In_ BOOL bEnable
    );

    [[nodiscard]]
    BOOL RtlProcessIsExited(
        _In_ HANDLE hProcess
    );

    [[nodiscard]]
    DWORD RtlProcessGetParentProcessId(
        _In_ HANDLE hProcess
    );
}

