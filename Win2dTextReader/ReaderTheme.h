#pragma once
#include "ReaderTheme.g.h"


namespace winrt::Win2dTextReader::implementation
{
    struct ReaderTheme : ReaderThemeT<ReaderTheme>
    {
        ReaderTheme() = default;

        ReaderTheme(int32_t index, hstring const& displayName);
        winrt::Microsoft::UI::Xaml::Media::Brush FontColor() const;
        winrt::Microsoft::UI::Xaml::Media::Brush Background() const;
        hstring DisplayName() const;

    private: 
        int32_t m_index; 
        hstring m_displayName; 
    };
}
namespace winrt::Win2dTextReader::factory_implementation
{
    struct ReaderTheme : ReaderThemeT<ReaderTheme, implementation::ReaderTheme>
    {
    };
}
