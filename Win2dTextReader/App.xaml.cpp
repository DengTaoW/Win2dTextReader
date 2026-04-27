#include "pch.h"
#include "App.xaml.h"
#include "MainWindow.xaml.h"


namespace winrt::Win2dTextReader::implementation
{
    App::App()
    {

#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BREAK_ON_UNHANDLED_EXCEPTION
        UnhandledException({ this, &App::OnDebugErrorCaptured });
#endif
    }

    void App::InitializeComponent()
    {
        AppT::InitializeComponent(); 
    }

    void App::OnLaunched(winrt::Microsoft::UI::Xaml::LaunchActivatedEventArgs const&)
    {
        m_window = winrt::Win2dTextReader::MainWindow(); 
        m_window.Activate();
    }

    void App::OnDebugErrorCaptured(
        winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::UnhandledExceptionEventArgs const& e)
    {
        if (IsDebuggerPresent())
        {
            auto errorMessage = e.Message();
            __debugbreak();
        }
    }
}
