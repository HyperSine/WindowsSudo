#pragma once

//
// Derived from https://github.com/processhacker/phnt/blob/master/ntseapi.h
//
#include <windows.h>
#include <winternl.h>

#pragma comment(lib, "ntdll")

EXTERN_C_START

    typedef struct _TOKEN_SECURITY_ATTRIBUTE_FQBN_VALUE {
        ULONG64 Version;
        UNICODE_STRING Name;
    } TOKEN_SECURITY_ATTRIBUTE_FQBN_VALUE, *PTOKEN_SECURITY_ATTRIBUTE_FQBN_VALUE;

    // private
    typedef struct _TOKEN_SECURITY_ATTRIBUTE_OCTET_STRING_VALUE {
        PVOID pValue;
        ULONG ValueLength;
    } TOKEN_SECURITY_ATTRIBUTE_OCTET_STRING_VALUE, *PTOKEN_SECURITY_ATTRIBUTE_OCTET_STRING_VALUE;

    typedef struct _TOKEN_SECURITY_ATTRIBUTE_V1 {
        UNICODE_STRING Name;
        USHORT ValueType;
        USHORT Reserved;
        ULONG Flags;
        ULONG ValueCount;
        union {
            PLONG64 pInt64;
            PULONG64 pUint64;
            PUNICODE_STRING pString;
            PTOKEN_SECURITY_ATTRIBUTE_FQBN_VALUE pFqbn;
            PTOKEN_SECURITY_ATTRIBUTE_OCTET_STRING_VALUE pOctetString;
        } Values;
    } TOKEN_SECURITY_ATTRIBUTE_V1, *PTOKEN_SECURITY_ATTRIBUTE_V1;

    typedef struct _TOKEN_SECURITY_ATTRIBUTES_INFORMATION {
        USHORT Version;
        USHORT Reserved;
        ULONG AttributeCount;
        union {
            PTOKEN_SECURITY_ATTRIBUTE_V1 pAttributeV1;
        } Attribute;
    } TOKEN_SECURITY_ATTRIBUTES_INFORMATION, *PTOKEN_SECURITY_ATTRIBUTES_INFORMATION;

    NTSYSCALLAPI
    NTSTATUS NTAPI NtCreateTokenEx(
        _Out_ PHANDLE TokenHandle,
        _In_ ACCESS_MASK DesiredAccess,
        _In_opt_ POBJECT_ATTRIBUTES ObjectAttributes,
        _In_ TOKEN_TYPE TokenType,
        _In_ PLUID AuthenticationId,
        _In_ PLARGE_INTEGER ExpirationTime,
        _In_ PTOKEN_USER User,
        _In_ PTOKEN_GROUPS Groups,
        _In_ PTOKEN_PRIVILEGES Privileges,
        _In_opt_ PTOKEN_SECURITY_ATTRIBUTES_INFORMATION UserAttributes,
        _In_opt_ PTOKEN_SECURITY_ATTRIBUTES_INFORMATION DeviceAttributes,
        _In_opt_ PTOKEN_GROUPS DeviceGroups,
        _In_opt_ PTOKEN_MANDATORY_POLICY TokenMandatoryPolicy,
        _In_opt_ PTOKEN_OWNER Owner,
        _In_ PTOKEN_PRIMARY_GROUP PrimaryGroup,
        _In_opt_ PTOKEN_DEFAULT_DACL DefaultDacl,
        _In_ PTOKEN_SOURCE TokenSource
    );

EXTERN_C_END