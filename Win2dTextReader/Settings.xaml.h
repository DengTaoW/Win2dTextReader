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
        winrt::Windows::Foundation::Collections::IPropertySet m_localSettings{ nullptr };
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::Windows::Foundation::IInspectable> m_fontsizeValues{ nullptr }; 
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::Windows::Foundation::IInspectable> m_lineHeightValues{ nullptr }; 

        template<typename T>
        T GetValue(winrt::hstring key)
        {
            winrt::Windows::Foundation::IInspectable object = m_localSettings.Lookup(key);
            return winrt::unbox_value<T>(object);
        }

        template<typename T>
        void SaveValue(const T& value, winrt::hstring key)
        {
            winrt::Windows::Foundation::IInspectable object = winrt::box_value(value);
            m_localSettings.Insert(key, object);
        }

        static winrt::hstring Format(double value, int digits); 

    public:
        Settings();

        void InitializeComponent();
        void SetFontSizeValues(); 
        void SetLineHeightValues(); 
        bool Load(); 
        void Save(); 
        void UpdateUI(); 

        double ReaderLineHeight() const;
        void ReaderLineHeight(double value);

        double ActualLineHeight() const { return m_lineHeight * m_fontSize; }

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

        void FontSizeComboBox_SelectionChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Controls::SelectionChangedEventArgs const& e);
        void LineHeightComboBox_SelectionChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Controls::SelectionChangedEventArgs const& e);
        void FontFamilyComboBox_SelectionChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Controls::SelectionChangedEventArgs const& e);
    };
}

namespace winrt::Win2dTextReader::factory_implementation
{
    struct Settings : SettingsT<Settings, implementation::Settings>
    {
    };
}
