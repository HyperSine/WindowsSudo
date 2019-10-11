#pragma once
#include <userenv.h>
#include <system_error>

#pragma comment(lib, "userenv")

namespace WSC {

    struct ResourceTraitsEnvironmentBlock {
        using HandleType = PVOID;

        static inline const HandleType InvalidValue = NULL;

        [[nodiscard]]
        static bool IsValid(const HandleType& Handle) noexcept {
            return Handle != InvalidValue;
        }

        static void Release(const HandleType& Handle) {
            if (DestroyEnvironmentBlock(Handle) == FALSE) {
                auto err = GetLastError();
                throw std::system_error(err, std::system_category());
            }
        }
    };

}
