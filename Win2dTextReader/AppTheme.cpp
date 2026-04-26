#include "pch.h"
#include "AppTheme.h"
#include "AppTheme.g.cpp"


namespace winrt::Win2dTextReader::implementation
{
    winrt::Microsoft::UI::Xaml::DependencyProperty AppTheme::s_backgroundProperty{ nullptr }; 
    winrt::Microsoft::UI::Xaml::DependencyProperty AppTheme::s_foregroundProperty{ nullptr }; 
    winrt::Microsoft::UI::Xaml::DependencyProperty AppTheme::s_nameProperty{ nullptr }; 

    winrt::Microsoft::UI::Xaml::DependencyProperty AppTheme::BackgroundProperty()
    {
        if (s_backgroundProperty == nullptr) {
            s_backgroundProperty = winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"Background",
                winrt::helper::GetTypeName<winrt::Microsoft::UI::Xaml::Media::Brush>(),
                winrt::xaml_typename<winrt::Win2dTextReader::AppTheme>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush(winrt::Windows::UI::Colors::Black()))
            );
        }

        return s_backgroundProperty; 
    }

    winrt::Microsoft::UI::Xaml::DependencyProperty AppTheme::ForegroundProperty()
    {
        if (s_foregroundProperty == nullptr) {
            s_foregroundProperty = winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"Foreground",
                winrt::helper::GetTypeName<winrt::Microsoft::UI::Xaml::Media::Brush>(),
                winrt::xaml_typename<winrt::Win2dTextReader::AppTheme>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush(winrt::Windows::UI::Colors::White()))
            );
        }

        return s_foregroundProperty;
    }

    winrt::Microsoft::UI::Xaml::DependencyProperty AppTheme::NameProperty()
    {
        if (s_nameProperty == nullptr) {
            s_nameProperty = winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
                L"Name",
                winrt::helper::GetTypeName<winrt::hstring>(),
                winrt::xaml_typename<winrt::Win2dTextReader::AppTheme>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata(winrt::box_value(L"Unknown"))
            ); 
        }

        return s_nameProperty; 
    }

    winrt::Microsoft::UI::Xaml::Media::Brush AppTheme::Background()
    {
        winrt::Windows::Foundation::IInspectable object = this->GetValue(s_backgroundProperty); 
        return object.as<winrt::Microsoft::UI::Xaml::Media::Brush>(); 
    }
    void AppTheme::Background(winrt::Microsoft::UI::Xaml::Media::Brush const& value)
    {
        if (s_backgroundProperty == nullptr) {
            s_backgroundProperty = AppTheme::BackgroundProperty(); 
        }
        this->SetValue(s_backgroundProperty, value); 
    }

    winrt::Microsoft::UI::Xaml::Media::Brush AppTheme::Foreground()
    {
        winrt::Windows::Foundation::IInspectable object = this->GetValue(s_foregroundProperty); 
        return object.as<winrt::Microsoft::UI::Xaml::Media::Brush>(); 
    }
    void AppTheme::Foreground(winrt::Microsoft::UI::Xaml::Media::Brush const& value)
    {
        if (s_foregroundProperty == nullptr) {
            s_foregroundProperty = AppTheme::ForegroundProperty();
        }
        this->SetValue(s_foregroundProperty, value); 
    }

    hstring AppTheme::Name()
    {
        winrt::Windows::Foundation::IInspectable object = this->GetValue(s_nameProperty); 
        return winrt::unbox_value<winrt::hstring>(object); 
    }
    void AppTheme::Name(hstring const& value)
    {
        if (s_nameProperty == nullptr) {
            s_nameProperty = AppTheme::NameProperty(); 
        }

        this->SetValue(s_nameProperty, winrt::box_value(value)); 
    }
}
