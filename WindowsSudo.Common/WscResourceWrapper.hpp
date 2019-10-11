#pragma once
#include <type_traits>
#include <utility>

namespace WSC {

    template<typename __ResourceTraits>
    class ResourceWrapper {
    public:

        using HandleType = typename __ResourceTraits::HandleType;
        static_assert(std::is_trivial_v<HandleType> && std::is_standard_layout_v<HandleType>);

    private:
    
        HandleType m_Handle;

    public:

        ResourceWrapper() noexcept :
            m_Handle(__ResourceTraits::InvalidValue) {}

        ResourceWrapper(const HandleType& Handle) noexcept :
            m_Handle(Handle) {}

        ResourceWrapper(__ResourceTraits) noexcept :
            m_Handle(__ResourceTraits::InvalidValue) {}

        ResourceWrapper(__ResourceTraits, const HandleType& Handle) noexcept :
            m_Handle(Handle) {}

        ResourceWrapper(const ResourceWrapper& Other) = delete;

        ResourceWrapper(ResourceWrapper&& Other) noexcept :
            m_Handle(std::move(Other.m_Handle)) { Other.m_Handle = __ResourceTraits::InvalidValue; }

        ResourceWrapper& operator=(const ResourceWrapper& Other) = delete;

        ResourceWrapper& operator=(ResourceWrapper&& Other) noexcept {
            if (this != std::addressof(Other)) {
                if (IsValid()) {
                    __ResourceTraits::Release(m_Handle);
                }

                m_Handle = std::move(Other.m_Handle);
                Other.m_Handle = __ResourceTraits::InvalidValue;
            }

            return *this;
        }

        [[nodiscard]]
        operator HandleType() const noexcept {  // NOLINT: Allow implicit conversion.
            return m_Handle;
        }

        template<typename __AsType>
        [[nodiscard]]
        __AsType As() const noexcept {
            return reinterpret_cast<__AsType>(m_Handle);
        }

        template<bool __Enable = std::is_pointer_v<HandleType>>
        [[nodiscard]]
        std::enable_if_t<__Enable, HandleType> operator->() const noexcept {
            return m_Handle;
        }

        [[nodiscard]]
        bool IsValid() const noexcept {
            return __ResourceTraits::IsValid(m_Handle);
        }

        [[nodiscard]]
        HandleType Get() const noexcept {
            return m_Handle;
        }

        [[nodiscard]]
        HandleType* GetAddressOf() noexcept {
            return &m_Handle;
        }

        template<typename __ReturnType>
        [[nodiscard]]
        __ReturnType GetAddressOfAs() noexcept {
            return reinterpret_cast<__ReturnType>(&m_Handle);
        }

        void TakeOver(const HandleType& Handle) {
            if (IsValid()) {
                __ResourceTraits::Release(m_Handle);
            }

            m_Handle = Handle;
        }

        void Discard() noexcept {
            m_Handle = __ResourceTraits::InvalidValue;
        }

        [[nodiscard]]
        HandleType Transfer() noexcept {
            HandleType t = m_Handle;
            m_Handle = __ResourceTraits::InvalidValue;
            return t;
        }

        template<typename __ReturnType>
        [[nodiscard]]
        __ReturnType TransferAs() noexcept {
            static_assert(std::is_trivial_v<__ReturnType> && std::is_standard_layout_v<__ReturnType>);

            __ReturnType t = reinterpret_cast<__ReturnType>(m_Handle);
            m_Handle = __ResourceTraits::InvalidValue;
            return t;
        }

        void Release() {
            if (IsValid()) {
                __ResourceTraits::Release(m_Handle);
                m_Handle = __ResourceTraits::InvalidValue;
            }
        }

        [[nodiscard]]
        HandleType* ReleaseAndGetAddressOf() {
            Release();
            return GetAddressOf();
        }

        template<typename __ReturnType>
        __ReturnType ReleaseAndGetAddressOfAs() {
            Release();
            return GetAddressOfAs<__ReturnType>();
        }

        ~ResourceWrapper() {
            Release();
        }
    };

    template<typename __ResourceTraits, typename __Deleter>
    class ResourceWrapperEx {
    public:

        using HandleType = typename __ResourceTraits::HandleType;
        using DeleterType = __Deleter;
        static_assert(std::is_pod_v<HandleType>);

    private:

        HandleType  m_Handle;
        DeleterType m_Deleter;

    public:

        template<typename __DeleterArgType>
        ResourceWrapperEx(__DeleterArgType&& Deleter) noexcept :
            m_Handle(__ResourceTraits::InvalidValue),
            m_Deleter(std::forward<__DeleterArgType>(Deleter)) {}

        template<typename __DeleterArgType>
        ResourceWrapperEx(const HandleType& Handle, __DeleterArgType&& Deleter) noexcept :
            m_Handle(Handle),
            m_Deleter(std::forward<__DeleterArgType>(Deleter)) {}

        template<typename __DeleterArgType>
        ResourceWrapperEx(__ResourceTraits, __DeleterArgType&& Deleter) noexcept :
            m_Handle(__ResourceTraits::InvalidValue),
            m_Deleter(std::forward<__DeleterArgType>(Deleter)) {}

        template<typename __DeleterArgType>
        ResourceWrapperEx(__ResourceTraits, const HandleType& Handle, __DeleterArgType&& Deleter) noexcept :
            m_Handle(Handle),
            m_Deleter(std::forward<__DeleterArgType>(Deleter)) {}

        ResourceWrapperEx(const ResourceWrapperEx& Other) = delete;

        ResourceWrapperEx(ResourceWrapperEx&& Other) noexcept :
            m_Handle(std::move(Other.m_Handle)),
            m_Deleter(std::move(Other.m_Deleter)) 
        {
            Other.m_Handle = __ResourceTraits::InvalidValue;
        }

        ResourceWrapperEx& operator=(const ResourceWrapperEx& Other) = delete;

        ResourceWrapperEx& operator=(ResourceWrapperEx&& Other) noexcept {
            if (this != std::addressof(Other)) {
                if (IsValid()) {
                    m_Deleter(m_Handle);
                }

                m_Handle = std::move(Other.m_Handle);
                m_Deleter = std::move(Other.m_Deleter);
                Other.m_Handle = __ResourceTraits::InvalidValue;
            }
        
            return *this;
        }

        [[nodiscard]]
        operator HandleType() const noexcept { // NOLINT: Allow implicit conversion.
            return m_Handle;
        }

        template<typename __AsType>
        [[nodiscard]]
        __AsType As() const noexcept {
            return reinterpret_cast<__AsType>(m_Handle);
        }

        template<bool __Enable = std::is_pointer_v<HandleType>>
        [[nodiscard]]
        std::enable_if_t<__Enable, HandleType> operator->() const noexcept {
            return m_Handle;
        }

        [[nodiscard]]
        bool IsValid() const noexcept {
            return __ResourceTraits::IsValid(m_Handle);
        }

        [[nodiscard]]
        HandleType Get() const noexcept {
            return m_Handle;
        }

        [[nodiscard]]
        HandleType* GetAddressOf() noexcept {
            return &m_Handle;
        }

        template<typename __ReturnType>
        [[nodiscard]]
        __ReturnType GetAddressOfAs() noexcept {
            return reinterpret_cast<__ReturnType>(&m_Handle);
        }

        void TakeOver(const HandleType& Handle) {
            if (IsValid()) {
                m_Deleter(m_Handle);
            }

            m_Handle = Handle;
        }

        void Discard() noexcept {
            m_Handle = __ResourceTraits::InvalidValue;
        }

        [[nodiscard]]
        HandleType Transfer() noexcept {
            HandleType t = m_Handle;
            m_Handle = __ResourceTraits::InvalidValue;
            return t;
        }

        template<typename __ReturnType>
        [[nodiscard]]
        __ReturnType TransferAs() noexcept {
            static_assert(std::is_trivial_v<__ReturnType> && std::is_standard_layout_v<__ReturnType>);

            __ReturnType t = reinterpret_cast<__ReturnType>(m_Handle);
            m_Handle = __ResourceTraits::InvalidValue;
            return t;
        }

        void Release() {
            if (IsValid()) {
                m_Deleter(m_Handle);
                m_Handle = __ResourceTraits::InvalidValue;
            }
        }

        [[nodiscard]]
        HandleType* ReleaseAndGetAddressOf() {
            Release();
            return GetAddressOf();
        }

        template<typename __ReturnType>
        __ReturnType ReleaseAndGetAddressOfAs() {
            Release();
            return GetAddressOfAs<__ReturnType>();
        }

        ~ResourceWrapperEx() {
            Release();
        }
    };

    template<typename __ClassType>
    struct ResourceTraitsCppObject {
        using HandleType = __ClassType*;

        static inline const HandleType InvalidValue = nullptr;

        [[nodiscard]]
        static bool IsValid(const HandleType& Handle) noexcept {
            return Handle != InvalidValue;
        }

        static void Release(const HandleType& Handle) {
            delete Handle;
        }
    };

    template<typename __ElementType>
    struct ResourceTraitsCppArray {
        using HandleType = __ElementType*;

        static inline const HandleType InvalidValue = nullptr;

        [[nodiscard]]
        static bool IsValid(const HandleType& Handle) noexcept {
            return Handle != InvalidValue;
        }

        static void Release(const HandleType& Handle) {
            delete[] Handle;
        }
    };

}

