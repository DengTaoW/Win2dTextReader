#pragma once

#include "Settings.g.h"

namespace winrt::Win2dTextReader::implementation
{
    struct Settings : SettingsT<Settings>
    {
    private:
        winrt::Win2dTextReader::MainWindowViewModel m_viewModel{ nullptr }; 

    public:
        Settings() = default; 
        Settings(winrt::Win2dTextReader::MainWindowViewModel const& viewModel);

        void InitializeComponent();
        winrt::Win2dTextReader::MainWindowViewModel ViewModel(); 
    };
}

namespace winrt::Win2dTextReader::factory_implementation
{
    struct Settings : SettingsT<Settings, implementation::Settings>
    {
    };
}
