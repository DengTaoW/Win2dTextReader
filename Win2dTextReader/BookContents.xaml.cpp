#include "pch.h"
#include "BookContents.xaml.h"
#include "BookContents.g.cpp"


namespace winrt::Win2dTextReader::implementation
{
    BookContents::BookContents(winrt::Win2dTextReader::MainWindowViewModel const& viewModel)
        : m_viewModel {viewModel}
    {
    }

    void BookContents::InitializeComponent()
    {
        BookContentsT::InitializeComponent(); 
    }

    winrt::Win2dTextReader::MainWindowViewModel BookContents::ViewModel()
    {
        return m_viewModel;
    }

    void BookContents::UserControl_Loaded(
        winrt::Windows::Foundation::IInspectable const&, 
        winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
    {
        if (m_viewModel.CurrentChapter() != nullptr) {
            this->ContentsListView().ScrollIntoView(
                m_viewModel.CurrentChapter(),
                winrt::Microsoft::UI::Xaml::Controls::ScrollIntoViewAlignment::Leading
            );
        }
    }
}




