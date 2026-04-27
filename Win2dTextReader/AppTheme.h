#pragma once
#include "AppTheme.g.h"


namespace winrt::Win2dTextReader::implementation
{
    struct AppTheme : AppThemeT<AppTheme>
    {
        AppTheme() = default; 

        static winrt::Microsoft::UI::Xaml::DependencyProperty BackgroundProperty();
        static winrt::Microsoft::UI::Xaml::DependencyProperty ForegroundProperty();
        static winrt::Microsoft::UI::Xaml::DependencyProperty NameProperty();

        winrt::Microsoft::UI::Xaml::Media::Brush Background();
        void Background(winrt::Microsoft::UI::Xaml::Media::Brush const& value);

        winrt::Microsoft::UI::Xaml::Media::Brush Foreground();
        void Foreground(winrt::Microsoft::UI::Xaml::Media::Brush const& value);

        hstring Name();
        void Name(hstring const& value);

        hstring ToString(); 

    private: 
        static winrt::Microsoft::UI::Xaml::DependencyProperty s_backgroundProperty;
        static winrt::Microsoft::UI::Xaml::DependencyProperty s_foregroundProperty; 
        static winrt::Microsoft::UI::Xaml::DependencyProperty s_nameProperty; 
    };
}
namespace winrt::Win2dTextReader::factory_implementation
{
    struct AppTheme : AppThemeT<AppTheme, implementation::AppTheme>
    {
    };
}
