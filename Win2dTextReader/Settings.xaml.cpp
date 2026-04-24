#include "pch.h"
#include "Settings.xaml.h"
#if __has_include("Settings.g.cpp")
#include "Settings.g.cpp"
#endif


namespace winrt::Win2dTextReader::implementation
{

    Settings::Settings(winrt::Win2dTextReader::MainWindowViewModel const& viewModel)
        : m_viewModel {viewModel}
    {
    }

    void Settings::InitializeComponent()
	{
        SettingsT::InitializeComponent();
	}

    winrt::Win2dTextReader::MainWindowViewModel Settings::ViewModel()
    {
        return m_viewModel; 
    }
}






