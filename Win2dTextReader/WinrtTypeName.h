#pragma once
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <type_traits>

namespace winrt::helper // 换个专属小窝，避免冲突
{
    // 基础约束
    template<typename T>
    concept WinrtType = std::is_base_of_v<winrt::Windows::Foundation::IInspectable, T>;

    // 原始模板声明
    template<typename T>
    winrt::Windows::UI::Xaml::Interop::TypeName GetTypeName();

    // 针对 WinRT 类型的实现 (使用 decay_t 增加鲁棒性)
    template<WinrtType T>
    winrt::Windows::UI::Xaml::Interop::TypeName GetTypeName()
    {
        return winrt::xaml_typename<T>();
    }

    // 针对基础类型的快捷宏 (减少重复劳动，妹妹就不用写那么长啦)
#define REGISTER_PRIMITIVE_TYPE(CppType, WinrtName) \
    template<> \
    inline winrt::Windows::UI::Xaml::Interop::TypeName GetTypeName<CppType>() { \
        return { L#WinrtName, winrt::Windows::UI::Xaml::Interop::TypeKind::Primitive }; \
    }

    REGISTER_PRIMITIVE_TYPE(uint8_t, UInt8)
    REGISTER_PRIMITIVE_TYPE(int16_t, Int16)
    REGISTER_PRIMITIVE_TYPE(int32_t, Int32)
    REGISTER_PRIMITIVE_TYPE(int64_t, Int64)
    REGISTER_PRIMITIVE_TYPE(float, Single)
    REGISTER_PRIMITIVE_TYPE(double, Double)
    REGISTER_PRIMITIVE_TYPE(bool, Boolean)
    REGISTER_PRIMITIVE_TYPE(winrt::hstring, String)

#undef REGISTER_PRIMITIVE_TYPE
}