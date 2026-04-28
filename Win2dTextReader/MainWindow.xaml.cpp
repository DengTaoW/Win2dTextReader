#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif


namespace winrt::Win2dTextReader::implementation
{
	MainWindow::MainWindow()
	{
		m_viewModel = winrt::Win2dTextReader::MainWindowViewModel(); 
		m_chaptersChangedToken = this->ChaptersChanged({&m_viewModel , &MainWindowViewModel::OnChaptersChanged });
		m_viewModelPropertyChangedToken = m_viewModel.PropertyChanged({ this, &MainWindow::OnViewModelPropertyChanged });
	}

	void MainWindow::InitializeComponent()
	{
		MainWindowT::InitializeComponent(); 

		auto presenter = winrt::Microsoft::UI::Windowing::OverlappedPresenter::Create();
		presenter.SetBorderAndTitleBar(true, true);
		presenter.IsMinimizable(false);
		presenter.IsMaximizable(false);
		this->AppWindow().SetPresenter(presenter); 
		this->ExtendsContentIntoTitleBar(true);

		m_appWindowChangedToken = this->AppWindow().Changed({ this, &MainWindow::OnWindowPropertyChanged });

		m_bookContentsControl = winrt::Win2dTextReader::BookContents(m_viewModel); 
		m_settingsControl = winrt::Win2dTextReader::Settings(m_viewModel);

		this->ContentsPopup().Child(m_bookContentsControl);
		this->SettingsPopup().Child(m_settingsControl);
		this->LoadHistoryAsync(); 
	}

	winrt::fire_and_forget MainWindow::LoadHistoryAsync()
	{
		winrt::Windows::Graphics::RectInt32 windowRect{
			m_viewModel.WindowPositionX(),
			m_viewModel.WindowPositionY(),
			m_viewModel.WindowSizeW(),
			m_viewModel.WindowSizeH()
		};
		this->AppWindow().MoveAndResize(windowRect); 

		if (m_viewModel.CurrentBook() != nullptr) {
			co_await m_viewModel.CurrentBook().InitializeAsync();
			m_chaptersChanged(winrt::box_value(false));

			this->ContentScrollView().ScrollBy(0, m_viewModel.ReaderVerticalOffset()); 
		}
	}

	winrt::Win2dTextReader::MainWindowViewModel MainWindow::ViewModel()
	{
		return m_viewModel; 
	}


	winrt::Windows::Foundation::Rect MainWindow::GetPopupRegion(float wScale, float hScale)
	{
		winrt::Windows::Foundation::Numerics::float2 readerSize = this->ReaderRegion().ActualSize(); 
		float width = readerSize.x * wScale; 
		float height = readerSize.y * hScale; 

		if (width < 300.0f) {
			width = readerSize.x - 40.0f; 
		}
		
		if (width > 700.0f) {
			width = 700.0f; 
		}
		float x = (readerSize.x - width) / 2.0f; 
		float y = readerSize.y - height - 10.0f;

		return { x, y, width, height };
	}

	void MainWindow::OnWindowPropertyChanged(
		winrt::Microsoft::UI::Windowing::AppWindow const& appwindow, 
		winrt::Microsoft::UI::Windowing::AppWindowChangedEventArgs const args)
	{
		if (args.DidPositionChange()) {
			auto newPostion = appwindow.Position(); 
			m_viewModel.WindowPositionX(newPostion.X); 
			m_viewModel.WindowPositionY(newPostion.Y); 
		}

		if (args.DidSizeChange()) {
			auto newSize = appwindow.Size(); 
			m_viewModel.WindowSizeW(newSize.Width); 
			m_viewModel.WindowSizeH(newSize.Height); 
		}
	}

	winrt::fire_and_forget MainWindow::OnOpenButtonClicked(
		winrt::Windows::Foundation::IInspectable const&, 
		winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		winrt::Microsoft::Windows::Storage::Pickers::FileOpenPicker picker{ this->AppWindow().Id() }; 
		picker.FileTypeFilter().Append(L".txt"); 
		picker.SuggestedStartLocation(winrt::Microsoft::Windows::Storage::Pickers::PickerLocationId::Downloads); 

		winrt::Microsoft::Windows::Storage::Pickers::PickFileResult result =
			co_await picker.PickSingleFileAsync(); 

		if (result == nullptr)
			co_return; 
		
		m_viewModel.CurrentBook(winrt::Xuanwen::Novel::NovelBook{ result.Path() }); 
		co_await m_viewModel.CurrentBook().InitializeAsync();

		m_chaptersChanged(winrt::box_value(true)); 
	}


	void MainWindow::OnPreviousChapterButtonClicked(
		winrt::Windows::Foundation::IInspectable const&, 
		winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		if (m_viewModel.ChapterIndex() > 0) {
			auto index = m_viewModel.ChapterIndex() - 1; 
			m_viewModel.ChapterIndex(index); 
		}
	}

	void MainWindow::OnNextChapterButtonClicked(
		winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		auto index = m_viewModel.ChapterIndex() + 1; 
		m_viewModel.ChapterIndex(index); 
	}

	void MainWindow::ShowContents(
		winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		auto rect = this->GetPopupRegion(0.6f, 0.8f);

		m_bookContentsControl.Width(rect.Width);
		m_bookContentsControl.Height(rect.Height); 

		this->ContentsPopup().HorizontalOffset(rect.X); 
		this->ContentsPopup().VerticalOffset(rect.Y); 
		this->ContentsPopup().IsOpen(true);
	}

	void MainWindow::ShowSettings(
		winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		auto rect = this->GetPopupRegion(0.6f, 0.6f);

		m_settingsControl.Width(rect.Width);
		m_settingsControl.Height(rect.Height);

		this->SettingsPopup().HorizontalOffset(rect.X);
		this->SettingsPopup().VerticalOffset(rect.Y);
		this->SettingsPopup().IsOpen(true);
	}


	void MainWindow::Window_Closed(
		winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::WindowEventArgs const&)
	{
		m_viewModel.SaveData(); 

		m_viewModel.PropertyChanged(m_viewModelPropertyChangedToken); 
		this->ChaptersChanged(m_chaptersChangedToken); 
		this->AppWindow().Changed(m_appWindowChangedToken); 
	}

	void MainWindow::ContentScrollView_ViewChanged(
		winrt::Microsoft::UI::Xaml::Controls::ScrollView const&, winrt::Windows::Foundation::IInspectable const&)
	{
		double verticalOffset = this->ContentScrollView().VerticalOffset();
		m_viewModel.ReaderVerticalOffset(verticalOffset);
	}

	void MainWindow::OnReaderRegionScrollUp(
		winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		auto viewportHeight = this->ContentScrollView().ViewportHeight(); 
		auto lineHeight = m_viewModel.LineHeight();
		auto scrollAmount = std::floor(viewportHeight / lineHeight) * lineHeight; 

		auto verticalOffset = this->ContentScrollView().VerticalOffset(); 
		if (verticalOffset < scrollAmount) {
			scrollAmount = verticalOffset; 
		}

		this->ContentScrollView().ScrollBy(0.0, -scrollAmount);
	}

	void MainWindow::OnReaderRegionScrollDown(
		winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		auto viewportHeight = this->ContentScrollView().ViewportHeight();
		auto lineHeight = m_viewModel.LineHeight();
		auto scrollAmount = std::floor(viewportHeight / lineHeight) * lineHeight;

		auto restAmount = this->ContentScrollView().ScrollableHeight() - this->ContentScrollView().VerticalOffset();
		if (restAmount < 0) {
			return;
		}

		scrollAmount = restAmount > scrollAmount ? scrollAmount : restAmount;
		this->ContentScrollView().ScrollBy(0.0, scrollAmount);
	}

	winrt::event_token MainWindow::ChaptersChanged(winrt::Win2dTextReader::ObjectAction const& handler)
	{
		return m_chaptersChanged.add(handler);
	}

	void MainWindow::ChaptersChanged(winrt::event_token const& token) noexcept
	{
		m_chaptersChanged.remove(token);
	}

	void MainWindow::OnViewModelPropertyChanged(
		winrt::Windows::Foundation::IInspectable const& , 
		winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e)
	{
		winrt::hstring propertyName = e.PropertyName(); 

		if (propertyName == L"CurrentChapter") {
			this->ContentsPopup().IsOpen(false);
			++m_chaptersCounter; 
			if (m_chaptersCounter != 1) {
				this->ContentScrollView().ScrollTo(0, 0); 
			}
		}
	}
}


