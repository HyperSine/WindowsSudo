#pragma once
#include <windows.h>

namespace WSP {

    [[nodiscard]]
    HANDLE RtlTokenCreateForLogonSid(
        _In_ HANDLE hTokenTemplate, 
        _In_ PSID LogonSid
    );

}

