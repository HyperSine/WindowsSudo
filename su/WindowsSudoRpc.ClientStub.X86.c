

/* this ALWAYS GENERATED file contains the RPC client stubs */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 11:14:07 2038
 */
/* Compiler settings for WindowsSudoRpc.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#if !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_)


#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */

#pragma optimize("", off ) 

#include <string.h>

#include "WindowsSudoRpcX86.h"

#define TYPE_FORMAT_STRING_SIZE   55                                
#define PROC_FORMAT_STRING_SIZE   159                               
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


extern const WindowsSudoRpc_MIDL_TYPE_FORMAT_STRING WindowsSudoRpc__MIDL_TypeFormatString;
extern const WindowsSudoRpc_MIDL_PROC_FORMAT_STRING WindowsSudoRpc__MIDL_ProcFormatString;
extern const WindowsSudoRpc_MIDL_EXPR_FORMAT_STRING WindowsSudoRpc__MIDL_ExprFormatString;

#define GENERIC_BINDING_TABLE_SIZE   0            


/* Standard interface: WindowsSudoRpc, ver. 1.0,
   GUID={0xa0fe973e,0xeec7,0x4190,{0xac,0x8b,0x60,0x31,0x1b,0xb5,0x98,0x95}} */


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
    0,
    0x00000000
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

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&WindowsSudoRpc_StubDesc,
                  (PFORMAT_STRING) &WindowsSudoRpc__MIDL_ProcFormatString.Format[0],
                  ( unsigned char * )&RpcHandle);
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

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&WindowsSudoRpc_StubDesc,
                  (PFORMAT_STRING) &WindowsSudoRpc__MIDL_ProcFormatString.Format[82],
                  ( unsigned char * )&RpcHandle);
    return ( DWORD  )_RetVal.Simple;
    
}


#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif
#if !(TARGET_IS_NT60_OR_LATER)
#error You need Windows Vista or later to run this stub because it uses these features:
#error   compiled for Windows Vista.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
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
/*  8 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 10 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 12 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 14 */	NdrFcShort( 0x10 ),	/* 16 */
/* 16 */	NdrFcShort( 0x24 ),	/* 36 */
/* 18 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x9,		/* 9 */
/* 20 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */
/* 24 */	NdrFcShort( 0x1 ),	/* 1 */
/* 26 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter hConsoleReference */

/* 28 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 30 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 32 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter lpszUserName */

/* 34 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 36 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 38 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter lpszApplicationName */

/* 40 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 42 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 44 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Parameter lpszCommandLine */

/* 46 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 48 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 50 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Parameter lpszCurrentDirectory */

/* 52 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 54 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 56 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Parameter cchEnvironment */

/* 58 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 60 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 62 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter lpszsEnvironment */

/* 64 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 66 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 68 */	NdrFcShort( 0xa ),	/* Type Offset=10 */

	/* Parameter phNewProcess */

/* 70 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 72 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 74 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 76 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 78 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 80 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure WsrCreateProcessAsElevated */

/* 82 */	0x0,		/* 0 */
			0x49,		/* Old Flags:  full ptr, */
/* 84 */	NdrFcLong( 0x0 ),	/* 0 */
/* 88 */	NdrFcShort( 0x1 ),	/* 1 */
/* 90 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 92 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 94 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 96 */	NdrFcShort( 0x10 ),	/* 16 */
/* 98 */	NdrFcShort( 0x24 ),	/* 36 */
/* 100 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x8,		/* 8 */
/* 102 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 104 */	NdrFcShort( 0x0 ),	/* 0 */
/* 106 */	NdrFcShort( 0x1 ),	/* 1 */
/* 108 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter hConsoleReference */

/* 110 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 112 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 114 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter lpszApplicationName */

/* 116 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 118 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 120 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Parameter lpszCommandLine */

/* 122 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 124 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 126 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Parameter lpszCurrentDirectory */

/* 128 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 130 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 132 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter cchEnvironment */

/* 134 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 136 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 138 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter lpszsEnvironment */

/* 140 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 142 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 144 */	NdrFcShort( 0x24 ),	/* Type Offset=36 */

	/* Parameter phNewProcess */

/* 146 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 148 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 150 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 152 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 154 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
/* 156 */	0x8,		/* FC_LONG */
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
/* 20 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 22 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 24 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 26 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 28 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 30 */	0x5,		/* FC_WCHAR */
			0x5b,		/* FC_END */
/* 32 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 34 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 36 */	
			0x1c,		/* FC_CVARRAY */
			0x1,		/* 1 */
/* 38 */	NdrFcShort( 0x2 ),	/* 2 */
/* 40 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x56,		/* FC_MULT_2 */
/* 42 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 44 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 46 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 48 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 50 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 52 */	0x5,		/* FC_WCHAR */
			0x5b,		/* FC_END */

			0x0
        }
    };

static const unsigned short WindowsSudoRpc_FormatStringOffsetTable[] =
    {
    0,
    82
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
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };
#pragma optimize("", on )
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_) */

