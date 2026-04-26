#pragma once

#include "App.xaml.g.h"

namespace winrt::Win2dTextReader::implementation
{
    struct App : AppT<App>
    {
        App();

        void InitializeComponent();

        void OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const&);
        void OnDebugErrorCaptured(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::UnhandledExceptionEventArgs const&);

    private:
        winrt::Microsoft::UI::Xaml::Window m_window{ nullptr };
        winrt::Win2dTextReader::AppTheme m_them1{ nullptr };
    };
}
