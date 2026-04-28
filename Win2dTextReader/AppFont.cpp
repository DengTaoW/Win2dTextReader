#include "pch.h"
#include "AppFont.h"
#include "AppFont.g.cpp"


namespace winrt::Win2dTextReader::implementation
{
    winrt::Microsoft::UI::Xaml::DependencyProperty AppFont::s_nameProperty{ nullptr }; 
    winrt::Microsoft::UI::Xaml::DependencyProperty AppFont::s_fontFamilyProperty{ nullptr }; 

    winrt::Microsoft::UI::Xaml::DependencyProperty AppFont::NameProperty()
    {
        if (s_nameProperty != nullptr)
            return s_nameProperty; 

        s_nameProperty = winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
            L"Name",
            winrt::helper::GetTypeName<winrt::hstring>(), 
            winrt::helper::GetTypeName<winrt::Win2dTextReader::AppFont>(), 
            winrt::Microsoft::UI::Xaml::PropertyMetadata(winrt::box_value(L""))
        );

        return s_nameProperty; 
    }

    winrt::Microsoft::UI::Xaml::DependencyProperty AppFont::FontFamilyProperty()
    {
        if (s_fontFamilyProperty != nullptr)
            return s_fontFamilyProperty; 

        s_fontFamilyProperty = winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
            L"FontFamily",
            winrt::helper::GetTypeName<winrt::Microsoft::UI::Xaml::Media::FontFamily>(),
            winrt::helper::GetTypeName<winrt::Win2dTextReader::AppFont>(),
            winrt::Microsoft::UI::Xaml::PropertyMetadata(winrt::Microsoft::UI::Xaml::Media::FontFamily(L"Microsoft YaHei UI"))
        ); 

        return s_fontFamilyProperty; 
    }

    hstring AppFont::Name()
    {
        winrt::Windows::Foundation::IInspectable object = this->GetValue(AppFont::NameProperty());
        return winrt::unbox_value<hstring>(object); 
    }

    void AppFont::Name(hstring const& value)
    {
        this->SetValue(AppFont::NameProperty(), winrt::box_value(value));
    }

    winrt::Microsoft::UI::Xaml::Media::FontFamily AppFont::FontFamily()
    {
        winrt::Windows::Foundation::IInspectable object = this->GetValue(AppFont::FontFamilyProperty()); 
        return object.as<winrt::Microsoft::UI::Xaml::Media::FontFamily>();
    }

    void AppFont::FontFamily(winrt::Microsoft::UI::Xaml::Media::FontFamily const& value)
    {
        this->SetValue(AppFont::FontFamilyProperty(), value); 
    }
}
