#include "WspRuntime.hpp"
#include "ntseapi_partial.h"
#include "WscResourceWrapper.hpp"
#include "WscResourceTraitsWin32.hpp"
#include "WscRuntime.hpp"

namespace WSP {

    [[nodiscard]]
    HANDLE RtlTokenCreateForLogonSid(_In_ HANDLE hTokenTemplate, _In_ PSID LogonSid) {
        WSC::ResourceWrapper<WSC::ResourceTraitsGenericHandle> hNewToken;

        WSC::ResourceWrapper<WSC::ResourceTraitsHeapAlloc> lpTokenStatistics;
        WSC::ResourceWrapper<WSC::ResourceTraitsHeapAlloc> lpTokenUser;
        WSC::ResourceWrapper<WSC::ResourceTraitsHeapAlloc> lpTokenGroups;
        WSC::ResourceWrapper<WSC::ResourceTraitsHeapAlloc> lpTokenPrivileges;
        WSC::ResourceWrapper<WSC::ResourceTraitsHeapAlloc> lpTokenDeviceGroups;
        WSC::ResourceWrapper<WSC::ResourceTraitsHeapAlloc> lpTokenMandatoryPolicy;
        WSC::ResourceWrapper<WSC::ResourceTraitsHeapAlloc> lpTokenOwner;
        WSC::ResourceWrapper<WSC::ResourceTraitsHeapAlloc> lpTokenPrimaryGroup;
        WSC::ResourceWrapper<WSC::ResourceTraitsHeapAlloc> lpTokenDefaultDacl;
        WSC::ResourceWrapper<WSC::ResourceTraitsHeapAlloc> lpTokenSource;

        WSC::ResourceWrapper<WSC::ResourceTraitsHeapAlloc> lpTokenLogonSid;
        WSC::ResourceWrapper<WSC::ResourceTraitsHeapAlloc> lpTokenSessionId;

        lpTokenStatistics.TakeOver(
            WSC::RtlTokenGetInformation(hTokenTemplate, TokenStatistics, NULL)
        );
        lpTokenUser.TakeOver(
            WSC::RtlTokenGetInformation(hTokenTemplate, TokenUser, NULL)
        );
        lpTokenGroups.TakeOver(
            WSC::RtlTokenGetInformation(hTokenTemplate, TokenGroups, NULL)
        );
        lpTokenPrivileges.TakeOver(
            WSC::RtlTokenGetInformation(hTokenTemplate, TokenPrivileges, NULL)
        );
        lpTokenDeviceGroups.TakeOver(
            WSC::RtlTokenGetInformation(hTokenTemplate, TokenDeviceGroups, NULL)
        );
        lpTokenMandatoryPolicy.TakeOver(
            WSC::RtlTokenGetInformation(hTokenTemplate, TokenMandatoryPolicy, NULL)
        );
        lpTokenOwner.TakeOver(
            WSC::RtlTokenGetInformation(hTokenTemplate, TokenOwner, NULL)
        );
        lpTokenPrimaryGroup.TakeOver(
            WSC::RtlTokenGetInformation(hTokenTemplate, TokenPrimaryGroup, NULL)
        );
        lpTokenDefaultDacl.TakeOver(
            WSC::RtlTokenGetInformation(hTokenTemplate, TokenDefaultDacl, NULL)
        );
        lpTokenSource.TakeOver(
            WSC::RtlTokenGetInformation(hTokenTemplate, TokenSource, NULL)
        );

        lpTokenLogonSid.TakeOver(
            WSC::RtlTokenGetInformation(hTokenTemplate, TokenLogonSid, NULL)
        );
        lpTokenSessionId.TakeOver(
            WSC::RtlTokenGetInformation(hTokenTemplate, TokenSessionId, NULL)
        );

        //
        // Source token should have one log-on SID only.
        //
        if (lpTokenLogonSid.As<PTOKEN_GROUPS>()->GroupCount != 1) {
            throw std::system_error(ERROR_ASSERTION_FAILURE, std::system_category());
        }

        {
            BOOL bRelplaced = FALSE;
            for (DWORD i = 0; i < lpTokenGroups.As<PTOKEN_GROUPS>()->GroupCount; ++i) {
                auto& Group = lpTokenGroups.As<PTOKEN_GROUPS>()->Groups[i];
                if (EqualSid(Group.Sid, lpTokenLogonSid.As<PTOKEN_GROUPS>()->Groups[0].Sid)) {
                    Group.Sid = LogonSid;
                    bRelplaced = TRUE;
                    break;
                }
            }

            if (bRelplaced == FALSE) {
                throw std::system_error(ERROR_NOT_FOUND, std::system_category());
            }
        }

        auto ntStatus = NtCreateTokenEx(
            hNewToken.GetAddressOf(),
            TOKEN_ALL_ACCESS,
            NULL,
            lpTokenStatistics.As<PTOKEN_STATISTICS>()->TokenType,
            &lpTokenStatistics.As<PTOKEN_STATISTICS>()->AuthenticationId,
            &lpTokenStatistics.As<PTOKEN_STATISTICS>()->ExpirationTime,
            lpTokenUser.As<PTOKEN_USER>(),
            lpTokenGroups.As<PTOKEN_GROUPS>(),
            lpTokenPrivileges.As<PTOKEN_PRIVILEGES>(),
            NULL,
            NULL,
            lpTokenDeviceGroups.As<PTOKEN_GROUPS>(),
            lpTokenMandatoryPolicy.As<PTOKEN_MANDATORY_POLICY>(),
            lpTokenOwner.As<PTOKEN_OWNER>(),
            lpTokenPrimaryGroup.As<PTOKEN_PRIMARY_GROUP>(),
            lpTokenDefaultDacl.As<PTOKEN_DEFAULT_DACL>(),
            lpTokenSource.As<PTOKEN_SOURCE>()
        );

        if (!NT_SUCCESS(ntStatus)) {
            throw std::system_error(RtlNtStatusToDosError(ntStatus), std::system_category());
        }

        WSC::RtlTokenSetInformation(hNewToken, TokenSessionId, lpTokenSessionId, sizeof(DWORD));

        return hNewToken.Transfer();
    }

}

