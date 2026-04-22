#include "pch.h"
#include "MainWindowViewModel.h"
#include "MainWindowViewModel.g.cpp"


namespace winrt::Win2dTextReader::implementation
{
	MainWindowViewModel::MainWindowViewModel()
	{
		m_fontItems = winrt::single_threaded_observable_vector<winrt::Win2dTextReader::FontItem>();
		this->InitializeCollections();

		auto appData = winrt::Microsoft::Windows::Storage::ApplicationData::GetDefault();
		m_localSettings = appData.LocalSettings().Values();
	}

	void MainWindowViewModel::LoadData()
	{
		std::vector<int> numbers{ 1, 2, 3, 4, 5 }; 
		auto numbersObject = winrt::Windows::Foundation::PropertyValue::CreateInt32Array(numbers); 
		
		// TODO
		/*
		保存的数据：
		CurrentBook.FilePath
		ChapterIndex
		WindowPositionX
		WindowPositionY
		WindowSizeX
		WindowSizeY
		ReaderVerticalOffset

		LineHeightIndex
		FontSizeIndex
		FontNameIndex
		*/
		throw hresult_not_implemented();
	}
	void MainWindowViewModel::SaveData()
	{
		throw hresult_not_implemented();
	}

	uint32_t MainWindowViewModel::ChapterIndex() const
	{
		return m_chapterIndex;
	}
	void MainWindowViewModel::ChapterIndex(uint32_t value)
	{
		if (value != m_chapterIndex) {
			m_chapterIndex = value;
			this->NotifyPropertyChanged(L"CurrentChapter");
		}
	}

	int32_t MainWindowViewModel::WindowPositionX() const
	{
		return m_windowPositionX;
	}
	void MainWindowViewModel::WindowPositionX(int32_t value)
	{
		if (value > 0) {
			m_windowPositionX = value;
		}
	}

	int32_t MainWindowViewModel::WindowPositionY() const
	{
		return m_windowPositionY;
	}
	void MainWindowViewModel::WindowPositionY(int32_t value)
	{
		if (value > 0) {
			m_windowPositionY = value;
		}
	}

	int32_t MainWindowViewModel::WindowSizeW() const
	{
		return m_windowSizeW;
	}
	void MainWindowViewModel::WindowSizeW(int32_t value)
	{
		if (value > 300) {
			m_windowSizeW = value;
		}
	}

	int32_t MainWindowViewModel::WindowSizeH() const
	{
		return m_windowSizeH;
	}
	void MainWindowViewModel::WindowSizeH(int32_t value)
	{
		if (value > 300) {
			m_windowSizeH = value;
		}
	}

	double MainWindowViewModel::ReaderVerticalOffset() const
	{
		return m_readerVerticalOffset;
	}
	void MainWindowViewModel::ReaderVerticalOffset(double value)
	{
		if (value >= 0) {
			m_readerVerticalOffset = value;
		}
	}

	winrt::Xuanwen::Novel::NovelBook MainWindowViewModel::CurrentBook()
	{
		return m_currentBook;
	}
	void MainWindowViewModel::CurrentBook(winrt::Xuanwen::Novel::NovelBook const& value)
	{
		m_currentBook = value;
	}

	winrt::Xuanwen::Novel::Chapter MainWindowViewModel::CurrentChapter()
	{
		if (m_currentBook == nullptr) {
			return nullptr;
		}

		auto totalChapters = m_currentBook.Chapters().Size();
		if (totalChapters == 0) {
			return nullptr;
		}

		if (m_chapterIndex >= totalChapters) {
			m_chapterIndex = 0;
		}

		return m_currentBook.Chapters().GetAt(m_chapterIndex);
	}

	double MainWindowViewModel::LineHeight() const
	{
		winrt::Win2dTextReader::DoubleItem item = m_lineHeightItems.GetAt(m_lineHeightIndex);
		return item.Value();
	}

	winrt::Windows::Foundation::Collections::IVector<winrt::Win2dTextReader::DoubleItem> MainWindowViewModel::LineHeightItems()
	{
		return m_lineHeightItems;
	}

	uint32_t MainWindowViewModel::LineHeightIndex() const
	{
		return m_lineHeightIndex;
	}

	void MainWindowViewModel::LineHeightIndex(uint32_t value)
	{
		if (m_lineHeightIndex != value) {
			m_lineHeightIndex = value;
			this->NotifyPropertyChanged(L"LineHeight");
		}
	}

	double MainWindowViewModel::FontSize() const
	{
		winrt::Win2dTextReader::DoubleItem item = m_fontSizeItems.GetAt(m_lineHeightIndex);
		return item.Value();
	}

	winrt::Windows::Foundation::Collections::IVector<winrt::Win2dTextReader::DoubleItem> MainWindowViewModel::FontSizeItems()
	{
		return m_fontSizeItems;
	}

	uint32_t MainWindowViewModel::FontSizeIndex() const
	{
		return m_fontSizeIndex;
	}

	void MainWindowViewModel::FontSizeIndex(uint32_t value)
	{
		if (value != m_fontSizeIndex) {
			m_fontSizeIndex = value;
			this->NotifyPropertyChanged(L"FontSize");
		}
	}

	winrt::Microsoft::UI::Xaml::Media::FontFamily MainWindowViewModel::FontFamily()
	{
		winrt::Win2dTextReader::FontItem item = m_fontItems.GetAt(m_fontFamilyIndex);
		return item.FontFamily();
	}

	winrt::Windows::Foundation::Collections::IVector<winrt::Win2dTextReader::FontItem> MainWindowViewModel::FontItems()
	{
		return m_fontItems;
	}

	uint32_t MainWindowViewModel::FontFamilyIndex() const
	{
		return m_fontFamilyIndex;
	}

	void MainWindowViewModel::FontFamilyIndex(uint32_t value)
	{
		if (m_fontFamilyIndex != value) {
			m_fontFamilyIndex = value;
			this->NotifyPropertyChanged(L"FontFamily");
		}
	}


	winrt::event_token MainWindowViewModel::PropertyChanged(winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
	{
		return m_propertyChanged.add(handler);
	}
	void MainWindowViewModel::PropertyChanged(winrt::event_token const& token) noexcept
	{
		m_propertyChanged.remove(token);
	}

	void MainWindowViewModel::NotifyPropertyChanged(std::wstring_view name)
	{
		m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(name));
	}

	void MainWindowViewModel::InitializeCollections()
	{
		double fontSize = 10.0;
		for (size_t i = 0; i < 8; i++) {
			winrt::Win2dTextReader::DoubleItem item(fontSize, 0);
			m_fontSizeItems.Append(item);
		}

		double lineHeight = 1.2;
		for (size_t i = 0; i < 7; i++) {
			winrt::Win2dTextReader::DoubleItem item(lineHeight, 1);
			m_lineHeightItems.Append(item);
		}

		std::map<std::wstring, std::wstring> fontNamesDict = {
			{L"宋体", L"SimSun"},
			{L"楷体", L"KaiTi"},
			{L"等线", L"DengXian"},
			{L"中黑", L"SimHei"},
			{L"雅黑", L"Microsoft YaHei"}
		};

		for (const auto& [displayName, fontName] : fontNamesDict) {
			winrt::Win2dTextReader::FontItem item(fontName, displayName); 
			m_fontItems.Append(item); 
		}
	}
}
