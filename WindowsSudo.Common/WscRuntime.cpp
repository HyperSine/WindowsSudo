#define UMDF_USING_NTSTATUS
#include <ntstatus.h>
#include "WscRuntime.hpp"
#include <winternl.h>
#include "WscResourceWrapper.hpp"
#include "WscResourceTraitsWin32.hpp"
#include <system_error>

#pragma comment(lib, "advapi32")
#pragma comment(lib, "ntdll")

namespace WSC {

    [[nodiscard]]
    BOOL RtlIsLocalSystem() {
        BOOL bResult;
        SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
        ResourceWrapper<ResourceTraitsSidAlloc> lpLocalSystemGroup;

        if (AllocateAndInitializeSid(&NtAuthority, 1, SECURITY_LOCAL_SYSTEM_RID, 0, 0, 0, 0, 0, 0, 0, lpLocalSystemGroup.GetAddressOf()) == FALSE) {
            auto err = GetLastError();
            throw std::system_error(err, std::system_category());
        }

        if (CheckTokenMembership(NULL, lpLocalSystemGroup, &bResult) == FALSE) {
            auto err = GetLastError();
            throw std::system_error(err, std::system_category());
        }

        return bResult;
    }

    [[nodiscard]]
    BOOL RtlObjectValidateType(_In_ HANDLE hObject, _In_ PCWSTR lpszObjectName) {
        NTSTATUS ntStatus;
        ULONG cbObjectTypeInfo = 0;
        ResourceWrapper<ResourceTraitsHeapAlloc> lpObjectTypeInfo;

        ntStatus = NtQueryObject(hObject, ObjectTypeInformation, NULL, 0, &cbObjectTypeInfo);
        if (ntStatus != STATUS_INFO_LENGTH_MISMATCH && !NT_SUCCESS(ntStatus)) {
            throw std::system_error(RtlNtStatusToDosError(ntStatus), std::system_category());
        }

        lpObjectTypeInfo.TakeOver(HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, cbObjectTypeInfo));
        if (lpObjectTypeInfo.IsValid() == false) {
            auto err = GetLastError();
            throw std::system_error(err, std::system_category());
        }

        ntStatus = NtQueryObject(hObject, ObjectTypeInformation, lpObjectTypeInfo, cbObjectTypeInfo, &cbObjectTypeInfo);
        if (!NT_SUCCESS(ntStatus)) {
            throw std::system_error(RtlNtStatusToDosError(ntStatus), std::system_category());
        }

        return _wcsicmp(lpObjectTypeInfo.As<PPUBLIC_OBJECT_TYPE_INFORMATION>()->TypeName.Buffer, lpszObjectName) == 0;
    }

    [[nodiscard]]
    PVOID RtlTokenGetInformation(_In_ HANDLE hToken, _In_ TOKEN_INFORMATION_CLASS TokenInformationClass, _Out_opt_ PDWORD pcbInformation) {
        DWORD cbBuffer = 0;
        ResourceWrapper<ResourceTraitsHeapAlloc> lpBuffer;

        if (GetTokenInformation(hToken, TokenInformationClass, 0, 0, &cbBuffer) == FALSE) {
            lpBuffer.TakeOver(HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, cbBuffer));
            if (lpBuffer.IsValid() == false) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }

            if (GetTokenInformation(hToken, TokenInformationClass, lpBuffer, cbBuffer, &cbBuffer) == FALSE) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }

            if (pcbInformation) {
                *pcbInformation = cbBuffer;
            }

            return lpBuffer.Transfer();
        } else {
            // of course, it will never throw.
            throw std::system_error(ERROR_ASSERTION_FAILURE, std::system_category());
        }
    }

    //
    // TokenOwner
    // TokenPrimaryGroup
    // TokenDefaultDacl
    // TokenSessionId
    // TokenSessionReference
    // TokenAuditPolicy
    // TokenOrigin
    // TokenLinkedToken
    // TokenVirtualizationAllowed
    // TokenVirtualizationEnabled
    // TokenIntegrityLevel
    // TokenUIAccess
    // TokenMandatoryPolicy
    // TokenSecurityAttributes
    // TokenPrivateNameSpace
    // TokenChildProcessFlags
    // 
    VOID RtlTokenSetInformation(_In_ HANDLE hToken, _In_ TOKEN_INFORMATION_CLASS TokenInformationClass, _In_ PVOID lpInformation, _In_ DWORD cbInformation) {
        if (SetTokenInformation(hToken, TokenInformationClass, lpInformation, cbInformation) == FALSE) {
            auto err = GetLastError();
            throw std::system_error(err, std::system_category());
        }
    }

    [[nodiscard]]
    BOOL RtlTokenIsElevated(_In_ HANDLE hToken) {
        TOKEN_ELEVATION_TYPE Info;

        if (DWORD _; GetTokenInformation(hToken, TokenElevationType, &Info, sizeof(TOKEN_ELEVATION_TYPE), &_) == FALSE) {
            auto err = GetLastError();
            throw std::system_error(err, std::system_category());
        }

        return Info != TokenElevationTypeLimited;
    }

    [[nodiscard]]
    BOOL RtlTokenCheckPrivilege(_In_ HANDLE hToken, _In_ PCTSTR lpszPrivilegeName, _Out_opt_ PBOOL pbEnable) {
        LUID TargetPrivilegeLuid;
        ResourceWrapper<ResourceTraitsHeapAlloc> lpPrivileges;

        if (LookupPrivilegeValue(NULL, lpszPrivilegeName, &TargetPrivilegeLuid) == FALSE) {
            auto err = GetLastError();
            throw std::system_error(err, std::system_category());
        }

        lpPrivileges.TakeOver(RtlTokenGetInformation(hToken, TokenPrivileges, NULL));

        for (DWORD i = 0; i < lpPrivileges.As<PTOKEN_PRIVILEGES>()->PrivilegeCount; ++i) {
            const LUID_AND_ATTRIBUTES& Privilege = lpPrivileges.As<PTOKEN_PRIVILEGES>()->Privileges[i];

            if (Privilege.Luid.LowPart == TargetPrivilegeLuid.LowPart && Privilege.Luid.HighPart == TargetPrivilegeLuid.HighPart) {
                if (pbEnable) {
                    *pbEnable = Privilege.Attributes & SE_PRIVILEGE_ENABLED ? TRUE : FALSE;
                }

                return TRUE;
            }
        }

        if (pbEnable) {
            *pbEnable = FALSE;
        }

        return FALSE;
    }

    [[nodiscard]]
    BOOL RtlTokenElevateIfPossible(_In_ HANDLE hToken, _Out_ PHANDLE phNewToken) {
        ResourceWrapper<ResourceTraitsHeapAlloc> lpElevationType;
        ResourceWrapper<ResourceTraitsHeapAlloc> lpLinkedToken;

        lpElevationType.TakeOver(RtlTokenGetInformation(hToken, TokenElevationType, NULL));
        if (*lpElevationType.As<PTOKEN_ELEVATION_TYPE>() == TokenElevationTypeLimited) {
            lpLinkedToken.TakeOver(RtlTokenGetInformation(hToken, TokenLinkedToken, NULL));
            *phNewToken = lpLinkedToken.As<PTOKEN_LINKED_TOKEN>()->LinkedToken;
            return TRUE;
        } else {
            *phNewToken = NULL;
            return FALSE;
        }
    }

    VOID RtlTokenModifyPrivilege(_In_ HANDLE hToken, _In_ PCTSTR lpszPrivilegeName, _In_ BOOL bEnable) {
        LUID PrivilegeLuid;
        TOKEN_PRIVILEGES PrivilegesToChange;

        if (LookupPrivilegeValue(NULL, lpszPrivilegeName, &PrivilegeLuid) == FALSE) {
            auto err = GetLastError();
            throw std::system_error(err, std::system_category());
        }

        PrivilegesToChange.PrivilegeCount = 1;
        PrivilegesToChange.Privileges[0].Luid = PrivilegeLuid;
        PrivilegesToChange.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;

        if (AdjustTokenPrivileges(hToken, FALSE, &PrivilegesToChange, sizeof(TOKEN_PRIVILEGES), NULL, NULL) == FALSE) {
            auto err = GetLastError();
            throw std::system_error(err, std::system_category());
        }

        if (GetLastError() == ERROR_NOT_ALL_ASSIGNED) {
            throw std::system_error(ERROR_NOT_ALL_ASSIGNED, std::system_category());
        }
    }

    [[nodiscard]]
    extern BOOL RtlProcessIsExited(_In_ HANDLE hProcess) {
        if (DWORD dwExitCode; GetExitCodeProcess(hProcess, &dwExitCode)) {
            return dwExitCode != STILL_ACTIVE;
        } else {
            auto err = GetLastError();
            throw std::system_error(err, std::system_category());
        }
    }

    [[nodiscard]]
    extern DWORD RtlProcessGetParentProcessId(_In_ HANDLE hProcess) {
        NTSTATUS ntStatus;
        PROCESS_BASIC_INFORMATION ProcessBasicInfo;

        if (ULONG _; ntStatus = NtQueryInformationProcess(hProcess, ProcessBasicInformation, &ProcessBasicInfo, sizeof(PROCESS_BASIC_INFORMATION), &_), !NT_SUCCESS(ntStatus)) {
            throw std::system_error(RtlNtStatusToDosError(ntStatus), std::system_category());
        }

        return static_cast<DWORD>(
            reinterpret_cast<ULONG_PTR>(ProcessBasicInfo.Reserved3)
        );
    }
}

