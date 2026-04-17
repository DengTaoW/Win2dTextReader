#include "pch.h"
#include "Settings.xaml.h"
#if __has_include("Settings.g.cpp")
#include "Settings.g.cpp"
#endif

// TODO 设计设置界面
// TODO 应用设置
// TODO 保存设置

namespace winrt::Win2dTextReader::implementation
{
	Settings::Settings()
        : m_lineHeight {0.0}
        , m_fontSize {11.0}
	{

	}

	void Settings::InitializeComponent()
	{
		SettingsT::InitializeComponent(); 
	}

    double Settings::ReaderLineHeight() const
    {
        return m_lineHeight; 
    }
    void Settings::ReaderLineHeight(double value)
    {
        if (m_lineHeight == value)
            return; 

        m_lineHeight = value; 
        m_propertyChanged(this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"ReaderLineHeight")); 
    }

    double Settings::ReaderFontSize() const
    {
        return m_fontSize; 
    }
    void Settings::ReaderFontSize(double value)
    {
        if (m_fontSize == value)
            return; 

        m_fontSize = value; 
        m_propertyChanged(this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"ReaderFontSize"));
    }

    winrt::Microsoft::UI::Xaml::Media::FontFamily Settings::ReaderFontFamily() const
    {
        return m_fontFamily; 
    }
    void Settings::ReaderFontFamily(winrt::Microsoft::UI::Xaml::Media::FontFamily const& value)
    {
        if (m_fontFamily.Source() == value.Source())
            return; 
        m_fontFamily = winrt::Microsoft::UI::Xaml::Media::FontFamily(value); 
        m_propertyChanged(this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"ReaderFontFamily"));
    }

    winrt::Windows::UI::Color Settings::ReaderForeground() const
    {
        return m_foreground; 
        
    }
    void Settings::ReaderForeground(winrt::Windows::UI::Color const& value)
    {
        if (m_foreground == value)
            return; 
        m_foreground = value;
        m_propertyChanged(this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"ReaderForeground"));
    }

    winrt::Microsoft::UI::Xaml::Media::Brush Settings::ReaderBackground() const
    {
        return m_background; 
    }
    void Settings::ReaderBackground(winrt::Microsoft::UI::Xaml::Media::Brush const& value)
    {
        if (m_background == value)
            return; 
        m_background = value; 
        m_propertyChanged(this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"ReaderBackground"));

    }

    winrt::event_token Settings::PropertyChanged(winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
    {
        return m_propertyChanged.add(handler); 
    }
    void Settings::PropertyChanged(winrt::event_token const& token) noexcept
    {
        m_propertyChanged.remove(token); 
    }
}
