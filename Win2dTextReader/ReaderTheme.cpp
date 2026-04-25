#include "pch.h"
#include "ReaderTheme.h"
#include "ReaderTheme.g.cpp"


namespace winrt::Win2dTextReader::implementation
{
    ReaderTheme::ReaderTheme(int32_t index, hstring const& displayName)
        : m_index {index}, m_displayName {displayName}
    {

    }

    winrt::Microsoft::UI::Xaml::Media::Brush ReaderTheme::FontColor() const
    {
        winrt::hstring key = L"FontColor_" + winrt::to_hstring(m_index); 

        auto resource = winrt::Microsoft::UI::Xaml::Application::Current().Resources(); 
        auto object = resource.TryLookup(winrt::box_value(key)); 
        if (object != nullptr) {
            return object.as<winrt::Microsoft::UI::Xaml::Media::Brush>(); 
        }
        return nullptr; 
    }

    winrt::Microsoft::UI::Xaml::Media::Brush ReaderTheme::Background() const
    {
        winrt::hstring key = L"AppBackground_" + winrt::to_hstring(m_index);

        auto resource = winrt::Microsoft::UI::Xaml::Application::Current().Resources();
        auto object = resource.TryLookup(winrt::box_value(key));
        if (object != nullptr) {
            return object.as<winrt::Microsoft::UI::Xaml::Media::Brush>();
        }
        return nullptr;
    }

    hstring ReaderTheme::DisplayName() const
    {
        return m_displayName; 
    }
}
