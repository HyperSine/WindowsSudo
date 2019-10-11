#pragma once
#include <windows.h>
#include <system_error>

namespace WSC {

    struct ResourceTraitsGenericHandle {
        using HandleType = HANDLE;

        static inline const HandleType InvalidValue = NULL;

        [[nodiscard]]
        static bool IsValid(const HandleType& Handle) noexcept {
            return Handle != InvalidValue;
        }

        static void Release(const HandleType& Handle) {
            if (CloseHandle(Handle) == FALSE) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }
        }
    };

    struct ResourceTraitsServiceHandle {
        using HandleType = SC_HANDLE;

        static inline const HandleType InvalidValue = NULL;

        [[nodiscard]]
        static bool IsValid(const HandleType& Handle) noexcept {
            return Handle != InvalidValue;
        }

        static void Release(const HandleType& Handle) {
            if (CloseServiceHandle(Handle) == FALSE) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }
        }
    };

    struct ResourceTraitsHeapAlloc {
        using HandleType = PVOID;

        static inline const HandleType InvalidValue = NULL;

        [[nodiscard]]
        static bool IsValid(const HandleType& Handle) noexcept {
            return Handle != InvalidValue;
        }

        static void Release(const HandleType& Handle) {
            if (HeapFree(GetProcessHeap(), 0, Handle) == FALSE) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }
        }
    };

    struct ResourceTraitsLocalAlloc {
        using HandleType = PVOID;

        static inline const HandleType InvalidValue = NULL;

        [[nodiscard]]
        static bool IsValid(const HandleType& Handle) noexcept {
            return Handle != InvalidValue;
        }

        static void Release(const HandleType& Handle) {
            if (LocalFree(Handle) != NULL) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }
        }
    };

    struct ResourceTraitsSidAlloc {
        using HandleType = PSID;

        static inline const HandleType InvalidValue = NULL;

        [[nodiscard]]
        static bool IsValid(const HandleType& Handle) noexcept {
            return Handle != InvalidValue;
        }

        static void Release(const HandleType& Handle) {
            if (FreeSid(Handle) != NULL) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }
        }
    };

}

