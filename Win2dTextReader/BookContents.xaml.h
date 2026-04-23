#pragma once
#include "BookContents.g.h"


namespace winrt::Win2dTextReader::implementation
{
    struct BookContents : BookContentsT<BookContents>
    {
    private:
        winrt::Win2dTextReader::MainWindowViewModel m_viewModel{ nullptr };

    public:
        BookContents() = default; 
        BookContents(winrt::Win2dTextReader::MainWindowViewModel const& viewModel); 

        void InitializeComponent(); 
        winrt::Win2dTextReader::MainWindowViewModel ViewModel();
        void UserControl_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
    };
}
namespace winrt::Win2dTextReader::factory_implementation
{
    struct BookContents : BookContentsT<BookContents, implementation::BookContents>
    {
    };
}
