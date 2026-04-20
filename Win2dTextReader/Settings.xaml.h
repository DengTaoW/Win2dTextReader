#pragma once

#include "Settings.g.h"

namespace winrt::Win2dTextReader::implementation
{
    struct Settings : SettingsT<Settings>
    {
    public:
        Settings();

        void InitializeComponent();
        void Save(); 

        double ReaderLineHeight() const;
        double ReaderFontSize() const;
        winrt::Microsoft::UI::Xaml::Media::FontFamily ReaderFontFamily() const;

        winrt::Windows::Foundation::Collections::IVector<winrt::Windows::Foundation::IInspectable> FontSizeValues();
        winrt::Windows::Foundation::Collections::IVector<winrt::Windows::Foundation::IInspectable> LineHeightValues();
        winrt::Windows::Foundation::Collections::IVector<winrt::Windows::Foundation::IInspectable> FontFamilyValues();

        winrt::event_token PropertyChanged(winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(winrt::event_token const& token) noexcept;

        void FontSizeComboBox_SelectionChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Controls::SelectionChangedEventArgs const& e);
        void LineHeightComboBox_SelectionChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Controls::SelectionChangedEventArgs const& e);
        void FontFamilyComboBox_SelectionChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Controls::SelectionChangedEventArgs const& e);

    private:
        static winrt::hstring Format(double x, int n); 

        int32_t m_fontSizeIndex{ 0 }; 
        int32_t m_lineHeightIndex{ 0 }; 
        int32_t m_fontFamilyIndex{ 0 }; 
        winrt::Windows::Foundation::Collections::IVector<winrt::Windows::Foundation::IInspectable> m_fontSizeValues{ nullptr }; 
        winrt::Windows::Foundation::Collections::IVector<winrt::Windows::Foundation::IInspectable> m_lineHeightValues{ nullptr }; 
        winrt::Windows::Foundation::Collections::IVector<winrt::Windows::Foundation::IInspectable> m_fontFamilyValues{ nullptr };

        winrt::event<winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;
        winrt::Windows::Foundation::Collections::IPropertySet m_localSettings{ nullptr };
        std::map<winrt::hstring, winrt::hstring> m_fontDict; 
    };
}

namespace winrt::Win2dTextReader::factory_implementation
{
    struct Settings : SettingsT<Settings, implementation::Settings>
    {
    };
}
