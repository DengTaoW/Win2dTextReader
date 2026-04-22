#pragma once
#include "FontItem.g.h"


namespace winrt::Win2dTextReader::implementation
{
    struct FontItem : FontItemT<FontItem>
    {
        FontItem() = default;

        FontItem(hstring const& fontFamily, hstring const& displayFontName);
        winrt::Microsoft::UI::Xaml::Media::FontFamily FontFamily();
        hstring DisplayFontName();

    private:
        hstring m_displayFontName; 
        winrt::Microsoft::UI::Xaml::Media::FontFamily m_fontFamily{ nullptr }; 
    };
}
namespace winrt::Win2dTextReader::factory_implementation
{
    struct FontItem : FontItemT<FontItem, implementation::FontItem>
    {
    };
}
