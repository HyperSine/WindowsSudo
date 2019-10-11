

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */


#ifndef __WindowsSudoRpcX86_h__
#define __WindowsSudoRpcX86_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

/* header files for imported files */
#include "WTypes.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __WindowsSudoRpc_INTERFACE_DEFINED__
#define __WindowsSudoRpc_INTERFACE_DEFINED__

/* interface WindowsSudoRpc */
/* [explicit_handle][endpoint][version][uuid] */ 

DWORD WsrCreateProcessWithLogon( 
    /* [in] */ handle_t RpcHandle,
    /* [in] */ ULONG_PTR hConsoleReference,
    /* [in] */ LPCWSTR lpszUserName,
    /* [full][in] */ LPCWSTR lpszApplicationName,
    /* [full][in] */ LPWSTR lpszCommandLine,
    /* [full][in] */ LPCWSTR lpszCurrentDirectory,
    /* [in] */ DWORD cchEnvironment,
    /* [length_is][size_is][full][in] */ const WCHAR lpszsEnvironment[  ],
    /* [out] */ ULONG_PTR *phNewProcess);

DWORD WsrCreateProcessAsElevated( 
    /* [in] */ handle_t RpcHandle,
    /* [in] */ ULONG_PTR hConsoleReference,
    /* [full][in] */ LPCWSTR lpszApplicationName,
    /* [full][in] */ LPWSTR lpszCommandLine,
    /* [in] */ LPCWSTR lpszCurrentDirectory,
    /* [in] */ DWORD cchEnvironment,
    /* [length_is][size_is][in] */ const WCHAR lpszsEnvironment[  ],
    /* [out] */ ULONG_PTR *phNewProcess);



extern RPC_IF_HANDLE WindowsSudoRpc_v1_0_c_ifspec;
extern RPC_IF_HANDLE WindowsSudoRpc_v1_0_s_ifspec;
#endif /* __WindowsSudoRpc_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


