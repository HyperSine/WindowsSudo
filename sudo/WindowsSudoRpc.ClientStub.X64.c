

/* this ALWAYS GENERATED file contains the RPC client stubs */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 11:14:07 2038
 */
/* Compiler settings for WindowsSudoRpc.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0622 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#if defined(_M_AMD64)


#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/

#include <string.h>

#include "WindowsSudoRpcX64.h"

#define TYPE_FORMAT_STRING_SIZE   55                                
#define PROC_FORMAT_STRING_SIZE   163                               
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _WindowsSudoRpc_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } WindowsSudoRpc_MIDL_TYPE_FORMAT_STRING;

typedef struct _WindowsSudoRpc_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } WindowsSudoRpc_MIDL_PROC_FORMAT_STRING;

typedef struct _WindowsSudoRpc_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } WindowsSudoRpc_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};

static const RPC_SYNTAX_IDENTIFIER  _NDR64_RpcTransferSyntax = 
{{0x71710533,0xbeba,0x4937,{0x83,0x19,0xb5,0xdb,0xef,0x9c,0xcc,0x36}},{1,0}};



extern const WindowsSudoRpc_MIDL_TYPE_FORMAT_STRING WindowsSudoRpc__MIDL_TypeFormatString;
extern const WindowsSudoRpc_MIDL_PROC_FORMAT_STRING WindowsSudoRpc__MIDL_ProcFormatString;
extern const WindowsSudoRpc_MIDL_EXPR_FORMAT_STRING WindowsSudoRpc__MIDL_ExprFormatString;

#define GENERIC_BINDING_TABLE_SIZE   0            


/* Standard interface: WindowsSudoRpc, ver. 1.0,
   GUID={0xa0fe973e,0xeec7,0x4190,{0xac,0x8b,0x60,0x31,0x1b,0xb5,0x98,0x95}} */

 extern const MIDL_STUBLESS_PROXY_INFO WindowsSudoRpc_ProxyInfo;

static const RPC_PROTSEQ_ENDPOINT __RpcProtseqEndpoint[] = 
    {
    {(unsigned char *) "ncalrpc", (unsigned char *) "WindowsSudoRpc"}
    };


static const RPC_CLIENT_INTERFACE WindowsSudoRpc___RpcClientInterface =
    {
    sizeof(RPC_CLIENT_INTERFACE),
    {{0xa0fe973e,0xeec7,0x4190,{0xac,0x8b,0x60,0x31,0x1b,0xb5,0x98,0x95}},{1,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    0,
    1,
    (RPC_PROTSEQ_ENDPOINT *)__RpcProtseqEndpoint,
    0,
    &WindowsSudoRpc_ProxyInfo,
    0x02000000
    };
RPC_IF_HANDLE WindowsSudoRpc_v1_0_c_ifspec = (RPC_IF_HANDLE)& WindowsSudoRpc___RpcClientInterface;

extern const MIDL_STUB_DESC WindowsSudoRpc_StubDesc;

static RPC_BINDING_HANDLE WindowsSudoRpc__MIDL_AutoBindHandle;


DWORD WsrCreateProcessWithLogon( 
    /* [in] */ handle_t RpcHandle,
    /* [in] */ ULONG_PTR hConsoleReference,
    /* [in] */ LPCWSTR lpszUserName,
    /* [full][in] */ LPCWSTR lpszApplicationName,
    /* [full][in] */ LPWSTR lpszCommandLine,
    /* [full][in] */ LPCWSTR lpszCurrentDirectory,
    /* [in] */ DWORD cchEnvironment,
    /* [length_is][size_is][full][in] */ const WCHAR lpszsEnvironment[  ],
    /* [out] */ ULONG_PTR *phNewProcess)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall3(
                  ( PMIDL_STUBLESS_PROXY_INFO  )&WindowsSudoRpc_ProxyInfo,
                  0,
                  0,
                  RpcHandle,
                  hConsoleReference,
                  lpszUserName,
                  lpszApplicationName,
                  lpszCommandLine,
                  lpszCurrentDirectory,
                  cchEnvironment,
                  lpszsEnvironment,
                  phNewProcess);
    return ( DWORD  )_RetVal.Simple;
    
}


DWORD WsrCreateProcessAsElevated( 
    /* [in] */ handle_t RpcHandle,
    /* [in] */ ULONG_PTR hConsoleReference,
    /* [full][in] */ LPCWSTR lpszApplicationName,
    /* [full][in] */ LPWSTR lpszCommandLine,
    /* [in] */ LPCWSTR lpszCurrentDirectory,
    /* [in] */ DWORD cchEnvironment,
    /* [length_is][size_is][in] */ const WCHAR lpszsEnvironment[  ],
    /* [out] */ ULONG_PTR *phNewProcess)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall3(
                  ( PMIDL_STUBLESS_PROXY_INFO  )&WindowsSudoRpc_ProxyInfo,
                  1,
                  0,
                  RpcHandle,
                  hConsoleReference,
                  lpszApplicationName,
                  lpszCommandLine,
                  lpszCurrentDirectory,
                  cchEnvironment,
                  lpszsEnvironment,
                  phNewProcess);
    return ( DWORD  )_RetVal.Simple;
    
}


#if !defined(__RPC_WIN64__)
#error  Invalid build platform for this stub.
#endif

static const WindowsSudoRpc_MIDL_PROC_FORMAT_STRING WindowsSudoRpc__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure WsrCreateProcessWithLogon */

			0x0,		/* 0 */
			0x49,		/* Old Flags:  full ptr, */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x0 ),	/* 0 */
/*  8 */	NdrFcShort( 0x50 ),	/* X64 Stack size/offset = 80 */
/* 10 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 12 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 14 */	NdrFcShort( 0x10 ),	/* 16 */
/* 16 */	NdrFcShort( 0x24 ),	/* 36 */
/* 18 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x9,		/* 9 */
/* 20 */	0xa,		/* 10 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */
/* 24 */	NdrFcShort( 0x1 ),	/* 1 */
/* 26 */	NdrFcShort( 0x0 ),	/* 0 */
/* 28 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter hConsoleReference */

/* 30 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 32 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 34 */	0xb9,		/* FC_UINT3264 */
			0x0,		/* 0 */

	/* Parameter lpszUserName */

/* 36 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 38 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 40 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter lpszApplicationName */

/* 42 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 44 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 46 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Parameter lpszCommandLine */

/* 48 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 50 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 52 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Parameter lpszCurrentDirectory */

/* 54 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 56 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 58 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Parameter cchEnvironment */

/* 60 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 62 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 64 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter lpszsEnvironment */

/* 66 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 68 */	NdrFcShort( 0x38 ),	/* X64 Stack size/offset = 56 */
/* 70 */	NdrFcShort( 0xa ),	/* Type Offset=10 */

	/* Parameter phNewProcess */

/* 72 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 74 */	NdrFcShort( 0x40 ),	/* X64 Stack size/offset = 64 */
/* 76 */	0xb9,		/* FC_UINT3264 */
			0x0,		/* 0 */

	/* Return value */

/* 78 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 80 */	NdrFcShort( 0x48 ),	/* X64 Stack size/offset = 72 */
/* 82 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure WsrCreateProcessAsElevated */

/* 84 */	0x0,		/* 0 */
			0x49,		/* Old Flags:  full ptr, */
/* 86 */	NdrFcLong( 0x0 ),	/* 0 */
/* 90 */	NdrFcShort( 0x1 ),	/* 1 */
/* 92 */	NdrFcShort( 0x48 ),	/* X64 Stack size/offset = 72 */
/* 94 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 96 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 98 */	NdrFcShort( 0x10 ),	/* 16 */
/* 100 */	NdrFcShort( 0x24 ),	/* 36 */
/* 102 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x8,		/* 8 */
/* 104 */	0xa,		/* 10 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 106 */	NdrFcShort( 0x0 ),	/* 0 */
/* 108 */	NdrFcShort( 0x1 ),	/* 1 */
/* 110 */	NdrFcShort( 0x0 ),	/* 0 */
/* 112 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter hConsoleReference */

/* 114 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 116 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 118 */	0xb9,		/* FC_UINT3264 */
			0x0,		/* 0 */

	/* Parameter lpszApplicationName */

/* 120 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 122 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 124 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Parameter lpszCommandLine */

/* 126 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 128 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 130 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Parameter lpszCurrentDirectory */

/* 132 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 134 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 136 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter cchEnvironment */

/* 138 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 140 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 142 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter lpszsEnvironment */

/* 144 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 146 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 148 */	NdrFcShort( 0x24 ),	/* Type Offset=36 */

	/* Parameter phNewProcess */

/* 150 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 152 */	NdrFcShort( 0x38 ),	/* X64 Stack size/offset = 56 */
/* 154 */	0xb9,		/* FC_UINT3264 */
			0x0,		/* 0 */

	/* Return value */

/* 156 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 158 */	NdrFcShort( 0x40 ),	/* X64 Stack size/offset = 64 */
/* 160 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const WindowsSudoRpc_MIDL_TYPE_FORMAT_STRING WindowsSudoRpc__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/*  4 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/*  6 */	
			0x14, 0x8,	/* FC_FP [simple_pointer] */
/*  8 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 10 */	
			0x14,		/* FC_FP */
			0x0,		/* 0 */
/* 12 */	NdrFcShort( 0x2 ),	/* Offset= 2 (14) */
/* 14 */	
			0x1c,		/* FC_CVARRAY */
			0x1,		/* 1 */
/* 16 */	NdrFcShort( 0x2 ),	/* 2 */
/* 18 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x56,		/* FC_MULT_2 */
/* 20 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 22 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 24 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 26 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 28 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 30 */	0x5,		/* FC_WCHAR */
			0x5b,		/* FC_END */
/* 32 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 34 */	0xb9,		/* FC_UINT3264 */
			0x5c,		/* FC_PAD */
/* 36 */	
			0x1c,		/* FC_CVARRAY */
			0x1,		/* 1 */
/* 38 */	NdrFcShort( 0x2 ),	/* 2 */
/* 40 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x56,		/* FC_MULT_2 */
/* 42 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 44 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 46 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 48 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 50 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 52 */	0x5,		/* FC_WCHAR */
			0x5b,		/* FC_END */

			0x0
        }
    };

static const unsigned short WindowsSudoRpc_FormatStringOffsetTable[] =
    {
    0,
    84
    };



#endif /* defined(_M_AMD64)*/



/* this ALWAYS GENERATED file contains the RPC client stubs */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 11:14:07 2038
 */
/* Compiler settings for WindowsSudoRpc.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0622 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#if defined(_M_AMD64)




#if !defined(__RPC_WIN64__)
#error  Invalid build platform for this stub.
#endif


#include "ndr64types.h"
#include "pshpack8.h"


typedef 
NDR64_FORMAT_CHAR
__midl_frag36_t;
extern const __midl_frag36_t __midl_frag36;

typedef 
NDR64_FORMAT_CHAR
__midl_frag35_t;
extern const __midl_frag35_t __midl_frag35;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag34_t;
extern const __midl_frag34_t __midl_frag34;

typedef 
struct 
{
    NDR64_FORMAT_UINT32 frag1;
    struct _NDR64_EXPR_NOOP frag2;
    struct _NDR64_EXPR_CONST64 frag3;
}
__midl_frag33_t;
extern const __midl_frag33_t __midl_frag33;

typedef 
struct 
{
    NDR64_FORMAT_UINT32 frag1;
    struct _NDR64_EXPR_VAR frag2;
}
__midl_frag32_t;
extern const __midl_frag32_t __midl_frag32;

typedef 
struct 
{
    NDR64_FORMAT_UINT32 frag1;
    struct _NDR64_EXPR_OPERATOR frag2;
    struct _NDR64_EXPR_VAR frag3;
    struct _NDR64_EXPR_CONST64 frag4;
}
__midl_frag31_t;
extern const __midl_frag31_t __midl_frag31;

typedef 
struct 
{
    struct _NDR64_CONF_VAR_ARRAY_HEADER_FORMAT frag1;
}
__midl_frag30_t;
extern const __midl_frag30_t __midl_frag30;

typedef 
struct _NDR64_CONFORMANT_STRING_FORMAT
__midl_frag28_t;
extern const __midl_frag28_t __midl_frag28;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag27_t;
extern const __midl_frag27_t __midl_frag27;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag25_t;
extern const __midl_frag25_t __midl_frag25;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
    struct _NDR64_PARAM_FORMAT frag7;
    struct _NDR64_PARAM_FORMAT frag8;
    struct _NDR64_PARAM_FORMAT frag9;
    struct _NDR64_PARAM_FORMAT frag10;
}
__midl_frag21_t;
extern const __midl_frag21_t __midl_frag21;

typedef 
struct 
{
    NDR64_FORMAT_UINT32 frag1;
    struct _NDR64_EXPR_VAR frag2;
}
__midl_frag16_t;
extern const __midl_frag16_t __midl_frag16;

typedef 
struct 
{
    NDR64_FORMAT_UINT32 frag1;
    struct _NDR64_EXPR_OPERATOR frag2;
    struct _NDR64_EXPR_VAR frag3;
    struct _NDR64_EXPR_CONST64 frag4;
}
__midl_frag15_t;
extern const __midl_frag15_t __midl_frag15;

typedef 
struct 
{
    struct _NDR64_CONF_VAR_ARRAY_HEADER_FORMAT frag1;
}
__midl_frag14_t;
extern const __midl_frag14_t __midl_frag14;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag13_t;
extern const __midl_frag13_t __midl_frag13;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
    struct _NDR64_PARAM_FORMAT frag7;
    struct _NDR64_PARAM_FORMAT frag8;
    struct _NDR64_PARAM_FORMAT frag9;
    struct _NDR64_PARAM_FORMAT frag10;
    struct _NDR64_PARAM_FORMAT frag11;
}
__midl_frag2_t;
extern const __midl_frag2_t __midl_frag2;

typedef 
NDR64_FORMAT_UINT32
__midl_frag1_t;
extern const __midl_frag1_t __midl_frag1;

static const __midl_frag36_t __midl_frag36 =
0x5    /* FC64_INT32 */;

static const __midl_frag35_t __midl_frag35 =
0x7    /* FC64_INT64 */;

static const __midl_frag34_t __midl_frag34 =
{ 
/* *ULONG_PTR */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 12 /* 0xc */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag35
};

static const __midl_frag33_t __midl_frag33 =
{ 
/*  */
    (NDR64_UINT32) 1 /* 0x1 */,
    { 
    /* struct _NDR64_EXPR_NOOP */
        0x5,    /* FC_EXPR_PAD */
        (NDR64_UINT8) 4 /* 0x4 */,
        (NDR64_UINT16) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_EXPR_CONST64 */
        0x2,    /* FC_EXPR_CONST64 */
        0x7,    /* FC64_INT64 */
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT64) 0 /* 0x0 */
    }
};

static const __midl_frag32_t __midl_frag32 =
{ 
/*  */
    (NDR64_UINT32) 1 /* 0x1 */,
    { 
    /* struct _NDR64_EXPR_VAR */
        0x3,    /* FC_EXPR_VAR */
        0x6,    /* FC64_UINT32 */
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT32) 40 /* 0x28 */  /* Offset */
    }
};

static const __midl_frag31_t __midl_frag31 =
{ 
/*  */
    (NDR64_UINT32) 1 /* 0x1 */,
    { 
    /* struct _NDR64_EXPR_OPERATOR */
        0x4,    /* FC_EXPR_OPER */
        0x10,    /* OP_STAR */
        0x0,    /* FC64_ZERO */
        (NDR64_UINT8) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_EXPR_VAR */
        0x3,    /* FC_EXPR_VAR */
        0x6,    /* FC64_UINT32 */
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT32) 40 /* 0x28 */  /* Offset */
    },
    { 
    /* struct _NDR64_EXPR_CONST64 */
        0x2,    /* FC_EXPR_CONST64 */
        0x7,    /* FC64_INT64 */
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT64) 2 /* 0x2 */
    }
};

static const __midl_frag30_t __midl_frag30 =
{ 
/*  */
    { 
    /* struct _NDR64_CONF_VAR_ARRAY_HEADER_FORMAT */
        0x43,    /* FC64_CONFVAR_ARRAY */
        (NDR64_UINT8) 1 /* 0x1 */,
        { 
        /* struct _NDR64_CONF_VAR_ARRAY_HEADER_FORMAT */
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 2 /* 0x2 */,
        &__midl_frag31,
        &__midl_frag32
    }
};

static const __midl_frag28_t __midl_frag28 =
{ 
/* *WCHAR */
    { 
    /* *WCHAR */
        0x64,    /* FC64_CONF_WCHAR_STRING */
        { 
        /* *WCHAR */
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT16) 2 /* 0x2 */
    }
};

static const __midl_frag27_t __midl_frag27 =
{ 
/* *WCHAR */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag28
};

static const __midl_frag25_t __midl_frag25 =
{ 
/* *WCHAR */
    0x23,    /* FC64_FP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag28
};

static const __midl_frag21_t __midl_frag21 =
{ 
/* WsrCreateProcessAsElevated */
    { 
    /* WsrCreateProcessAsElevated */      /* procedure WsrCreateProcessAsElevated */
        (NDR64_UINT32) 19664960 /* 0x12c1040 */,    /* explicit handle */ /* IsIntrepreted, [ptr], ClientMustSize, HasReturn, ServerCorrelation, HasExtensions */
        (NDR64_UINT32) 72 /* 0x48 */ ,  /* Stack size */
        (NDR64_UINT32) 24 /* 0x18 */,
        (NDR64_UINT32) 48 /* 0x30 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 8 /* 0x8 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x72,    /* FC64_BIND_PRIMITIVE */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 0 /* 0x0 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* hConsoleReference */      /* parameter hConsoleReference */
        &__midl_frag35,
        { 
        /* hConsoleReference */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* lpszApplicationName */      /* parameter lpszApplicationName */
        &__midl_frag25,
        { 
        /* lpszApplicationName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* lpszCommandLine */      /* parameter lpszCommandLine */
        &__midl_frag25,
        { 
        /* lpszCommandLine */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    },
    { 
    /* lpszCurrentDirectory */      /* parameter lpszCurrentDirectory */
        &__midl_frag28,
        { 
        /* lpszCurrentDirectory */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        32 /* 0x20 */,   /* Stack offset */
    },
    { 
    /* cchEnvironment */      /* parameter cchEnvironment */
        &__midl_frag36,
        { 
        /* cchEnvironment */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        40 /* 0x28 */,   /* Stack offset */
    },
    { 
    /* lpszsEnvironment */      /* parameter lpszsEnvironment */
        &__midl_frag30,
        { 
        /* lpszsEnvironment */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        48 /* 0x30 */,   /* Stack offset */
    },
    { 
    /* phNewProcess */      /* parameter phNewProcess */
        &__midl_frag35,
        { 
        /* phNewProcess */
            0,
            0,
            0,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            1
        },    /* [out], Basetype, SimpleRef, UseCache */
        (NDR64_UINT16) 0 /* 0x0 */,
        56 /* 0x38 */,   /* Stack offset */
    },
    { 
    /* DWORD */      /* parameter DWORD */
        &__midl_frag36,
        { 
        /* DWORD */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        64 /* 0x40 */,   /* Stack offset */
    }
};

static const __midl_frag16_t __midl_frag16 =
{ 
/*  */
    (NDR64_UINT32) 1 /* 0x1 */,
    { 
    /* struct _NDR64_EXPR_VAR */
        0x3,    /* FC_EXPR_VAR */
        0x6,    /* FC64_UINT32 */
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT32) 48 /* 0x30 */  /* Offset */
    }
};

static const __midl_frag15_t __midl_frag15 =
{ 
/*  */
    (NDR64_UINT32) 1 /* 0x1 */,
    { 
    /* struct _NDR64_EXPR_OPERATOR */
        0x4,    /* FC_EXPR_OPER */
        0x10,    /* OP_STAR */
        0x0,    /* FC64_ZERO */
        (NDR64_UINT8) 0 /* 0x0 */
    },
    { 
    /* struct _NDR64_EXPR_VAR */
        0x3,    /* FC_EXPR_VAR */
        0x6,    /* FC64_UINT32 */
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT32) 48 /* 0x30 */  /* Offset */
    },
    { 
    /* struct _NDR64_EXPR_CONST64 */
        0x2,    /* FC_EXPR_CONST64 */
        0x7,    /* FC64_INT64 */
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT64) 2 /* 0x2 */
    }
};

static const __midl_frag14_t __midl_frag14 =
{ 
/*  */
    { 
    /* struct _NDR64_CONF_VAR_ARRAY_HEADER_FORMAT */
        0x43,    /* FC64_CONFVAR_ARRAY */
        (NDR64_UINT8) 1 /* 0x1 */,
        { 
        /* struct _NDR64_CONF_VAR_ARRAY_HEADER_FORMAT */
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 2 /* 0x2 */,
        &__midl_frag15,
        &__midl_frag16
    }
};

static const __midl_frag13_t __midl_frag13 =
{ 
/* struct _NDR64_POINTER_FORMAT */
    0x23,    /* FC64_FP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag14
};

static const __midl_frag2_t __midl_frag2 =
{ 
/* WsrCreateProcessWithLogon */
    { 
    /* WsrCreateProcessWithLogon */      /* procedure WsrCreateProcessWithLogon */
        (NDR64_UINT32) 19664960 /* 0x12c1040 */,    /* explicit handle */ /* IsIntrepreted, [ptr], ClientMustSize, HasReturn, ServerCorrelation, HasExtensions */
        (NDR64_UINT32) 80 /* 0x50 */ ,  /* Stack size */
        (NDR64_UINT32) 24 /* 0x18 */,
        (NDR64_UINT32) 48 /* 0x30 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 9 /* 0x9 */,
        (NDR64_UINT16) 8 /* 0x8 */
    },
    { 
    /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
        { 
        /* struct _NDR64_BIND_AND_NOTIFY_EXTENSION */
            0x72,    /* FC64_BIND_PRIMITIVE */
            (NDR64_UINT8) 0 /* 0x0 */,
            0 /* 0x0 */,   /* Stack offset */
            (NDR64_UINT8) 0 /* 0x0 */,
            (NDR64_UINT8) 0 /* 0x0 */
        },
        (NDR64_UINT16) 0 /* 0x0 */      /* Notify index */
    },
    { 
    /* hConsoleReference */      /* parameter hConsoleReference */
        &__midl_frag35,
        { 
        /* hConsoleReference */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* lpszUserName */      /* parameter lpszUserName */
        &__midl_frag28,
        { 
        /* lpszUserName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], SimpleRef */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* lpszApplicationName */      /* parameter lpszApplicationName */
        &__midl_frag25,
        { 
        /* lpszApplicationName */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    },
    { 
    /* lpszCommandLine */      /* parameter lpszCommandLine */
        &__midl_frag25,
        { 
        /* lpszCommandLine */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        32 /* 0x20 */,   /* Stack offset */
    },
    { 
    /* lpszCurrentDirectory */      /* parameter lpszCurrentDirectory */
        &__midl_frag25,
        { 
        /* lpszCurrentDirectory */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        40 /* 0x28 */,   /* Stack offset */
    },
    { 
    /* cchEnvironment */      /* parameter cchEnvironment */
        &__midl_frag36,
        { 
        /* cchEnvironment */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        48 /* 0x30 */,   /* Stack offset */
    },
    { 
    /* lpszsEnvironment */      /* parameter lpszsEnvironment */
        &__midl_frag13,
        { 
        /* lpszsEnvironment */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in] */
        (NDR64_UINT16) 0 /* 0x0 */,
        56 /* 0x38 */,   /* Stack offset */
    },
    { 
    /* phNewProcess */      /* parameter phNewProcess */
        &__midl_frag35,
        { 
        /* phNewProcess */
            0,
            0,
            0,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            1
        },    /* [out], Basetype, SimpleRef, UseCache */
        (NDR64_UINT16) 0 /* 0x0 */,
        64 /* 0x40 */,   /* Stack offset */
    },
    { 
    /* DWORD */      /* parameter DWORD */
        &__midl_frag36,
        { 
        /* DWORD */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        72 /* 0x48 */,   /* Stack offset */
    }
};

static const __midl_frag1_t __midl_frag1 =
(NDR64_UINT32) 0 /* 0x0 */;


#include "poppack.h"


static const FormatInfoRef WindowsSudoRpc_Ndr64ProcTable[] =
    {
    &__midl_frag2,
    &__midl_frag21
    };


static const MIDL_STUB_DESC WindowsSudoRpc_StubDesc = 
    {
    (void *)& WindowsSudoRpc___RpcClientInterface,
    MIDL_user_allocate,
    MIDL_user_free,
    &WindowsSudoRpc__MIDL_AutoBindHandle,
    0,
    0,
    0,
    0,
    WindowsSudoRpc__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x60001, /* Ndr library version */
    0,
    0x801026e, /* MIDL Version 8.1.622 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    0x2000001, /* MIDL flag */
    0, /* cs routines */
    (void *)& WindowsSudoRpc_ProxyInfo,   /* proxy/server info */
    0
    };

static const MIDL_SYNTAX_INFO WindowsSudoRpc_SyntaxInfo [  2 ] = 
    {
    {
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    0,
    WindowsSudoRpc__MIDL_ProcFormatString.Format,
    WindowsSudoRpc_FormatStringOffsetTable,
    WindowsSudoRpc__MIDL_TypeFormatString.Format,
    0,
    0,
    0
    }
    ,{
    {{0x71710533,0xbeba,0x4937,{0x83,0x19,0xb5,0xdb,0xef,0x9c,0xcc,0x36}},{1,0}},
    0,
    0 ,
    (unsigned short *) WindowsSudoRpc_Ndr64ProcTable,
    0,
    0,
    0,
    0
    }
    };

static const MIDL_STUBLESS_PROXY_INFO WindowsSudoRpc_ProxyInfo =
    {
    &WindowsSudoRpc_StubDesc,
    WindowsSudoRpc__MIDL_ProcFormatString.Format,
    WindowsSudoRpc_FormatStringOffsetTable,
    (RPC_SYNTAX_IDENTIFIER*)&_RpcTransferSyntax,
    2,
    (MIDL_SYNTAX_INFO*)WindowsSudoRpc_SyntaxInfo
    
    };

#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* defined(_M_AMD64)*/

