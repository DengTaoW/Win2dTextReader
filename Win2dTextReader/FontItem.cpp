#include "pch.h"
#include "FontItem.h"
#include "FontItem.g.cpp"


namespace winrt::Win2dTextReader::implementation
{
    FontItem::FontItem( hstring const& fontFamily, hstring const& displayFontName)
        :m_displayFontName {displayFontName}
    {
        m_fontFamily = winrt::Microsoft::UI::Xaml::Media::FontFamily{ fontFamily };
    }

    winrt::Microsoft::UI::Xaml::Media::FontFamily FontItem::FontFamily()
    {
        return m_fontFamily;
    }

    hstring FontItem::DisplayFontName()
    {
        return m_displayFontName;
    }
}
