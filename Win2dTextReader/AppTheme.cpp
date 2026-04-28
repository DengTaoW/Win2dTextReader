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
                winrt::helper::GetTypeName<winrt::Win2dTextReader::AppTheme>(),
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
                winrt::helper::GetTypeName<winrt::Win2dTextReader::AppTheme>(),
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
                winrt::helper::GetTypeName<winrt::Win2dTextReader::AppTheme>(),
                winrt::Microsoft::UI::Xaml::PropertyMetadata(winrt::box_value(L"Unknown"))
            ); 
        }

        return s_nameProperty; 
    }

    winrt::Microsoft::UI::Xaml::Media::Brush AppTheme::Background()
    {
        winrt::Windows::Foundation::IInspectable object = this->GetValue(AppTheme::BackgroundProperty());
        return object.as<winrt::Microsoft::UI::Xaml::Media::Brush>(); 
    }

    void AppTheme::Background(winrt::Microsoft::UI::Xaml::Media::Brush const& value)
    {
        this->SetValue(AppTheme::BackgroundProperty(), value);
    }

    winrt::Microsoft::UI::Xaml::Media::Brush AppTheme::Foreground()
    {
        winrt::Windows::Foundation::IInspectable object = this->GetValue(AppTheme::ForegroundProperty());
        return object.as<winrt::Microsoft::UI::Xaml::Media::Brush>(); 
    }

    void AppTheme::Foreground(winrt::Microsoft::UI::Xaml::Media::Brush const& value)
    {
        this->SetValue(AppTheme::ForegroundProperty(), value);
    }

    hstring AppTheme::Name()
    {
        winrt::Windows::Foundation::IInspectable object = this->GetValue(AppTheme::NameProperty());
        return winrt::unbox_value<winrt::hstring>(object); 
    }

    void AppTheme::Name(hstring const& value)
    {
        this->SetValue(AppTheme::NameProperty(), winrt::box_value(value));
    }
}
