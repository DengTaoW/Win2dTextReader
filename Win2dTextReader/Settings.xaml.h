#pragma once

#include "Settings.g.h"

namespace winrt::Win2dTextReader::implementation
{
    struct Settings : SettingsT<Settings>
    {
    private:
        double m_lineHeight; 
        double m_fontSize; 
        winrt::Windows::UI::Color m_foreground; 
        winrt::Microsoft::UI::Xaml::Media::FontFamily m_fontFamily{ nullptr };
        winrt::Microsoft::UI::Xaml::Media::Brush m_background{ nullptr }; 

        winrt::event<winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged; 


    public:
        Settings();

        void InitializeComponent();

        double ReaderLineHeight() const;
        void ReaderLineHeight(double value);

        double ReaderFontSize() const;
        void ReaderFontSize(double value);

        winrt::Microsoft::UI::Xaml::Media::FontFamily ReaderFontFamily() const;
        void ReaderFontFamily(winrt::Microsoft::UI::Xaml::Media::FontFamily const& value);

        winrt::Windows::UI::Color ReaderForeground() const;
        void ReaderForeground(winrt::Windows::UI::Color const& value);

        winrt::Microsoft::UI::Xaml::Media::Brush ReaderBackground() const;
        void ReaderBackground(winrt::Microsoft::UI::Xaml::Media::Brush const& value);

        winrt::event_token PropertyChanged(winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(winrt::event_token const& token) noexcept;

    };
}

namespace winrt::Win2dTextReader::factory_implementation
{
    struct Settings : SettingsT<Settings, implementation::Settings>
    {
    };
}
