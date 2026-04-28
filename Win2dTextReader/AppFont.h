#pragma once
#include "AppFont.g.h"

namespace winrt::Win2dTextReader::implementation
{
    struct AppFont : AppFontT<AppFont>
    {
        AppFont() = default;

        static winrt::Microsoft::UI::Xaml::DependencyProperty NameProperty();
        static winrt::Microsoft::UI::Xaml::DependencyProperty FontFamilyProperty();

        hstring Name();
        void Name(hstring const& value);

        winrt::Microsoft::UI::Xaml::Media::FontFamily FontFamily();
        void FontFamily(winrt::Microsoft::UI::Xaml::Media::FontFamily const& value);

    private: 
        static winrt::Microsoft::UI::Xaml::DependencyProperty s_nameProperty; 
        static winrt::Microsoft::UI::Xaml::DependencyProperty s_fontFamilyProperty; 
    };
}
namespace winrt::Win2dTextReader::factory_implementation
{
    struct AppFont : AppFontT<AppFont, implementation::AppFont>
    {
    };
}
