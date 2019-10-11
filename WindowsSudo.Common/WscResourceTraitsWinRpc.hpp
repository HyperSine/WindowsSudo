#pragma once
#include <rpc.h>
#include <system_error>

#pragma comment(lib, "rpcrt4")

namespace WSC {

    struct ResourceTraitsRpcStringA {
        using HandleType = RPC_CSTR;

        static inline const HandleType InvalidValue = NULL;

        [[nodiscard]]
        static bool IsValid(const HandleType& Handle) noexcept {
            return Handle != InvalidValue;
        }

        static void Release(HandleType& Handle) {
            if (auto RpcStatus = RpcStringFreeA(&Handle); RpcStatus != RPC_S_OK) {
                throw std::system_error(RpcStatus, std::system_category());
            }
        }
    };

    struct ResourceTraitsRpcStringW {
        using HandleType = RPC_WSTR;

        static inline const HandleType InvalidValue = NULL;

        [[nodiscard]]
        static bool IsValid(const HandleType& Handle) noexcept {
            return Handle != InvalidValue;
        }

        static void Release(HandleType& Handle) {
            if (auto RpcStatus = RpcStringFreeW(&Handle); RpcStatus != RPC_S_OK) {
                throw std::system_error(RpcStatus, std::system_category());
            }
        }
    };

    struct ResourceTraitsRpcBindHandle {
        using HandleType = RPC_BINDING_HANDLE;

        static inline const HandleType InvalidValue = NULL;

        [[nodiscard]]
        static bool IsValid(const HandleType& Handle) noexcept {
            return Handle != InvalidValue;
        }

        static void Release(const HandleType& Handle) {
            if (auto RpcStatus = RpcBindingUnbind(Handle); RpcStatus != RPC_S_OK) {
                throw std::system_error(RpcStatus, std::system_category());
            }
        }
    };

}

