#include "pch.h"
#include "ReadingHistory.h"
#include "ReadingHistory.g.cpp"


namespace winrt::Win2dTextReader::implementation
{
	constexpr wchar_t READING_HISTORY_KEY[]{ L"RH" };

	constexpr wchar_t FILE_PATH_KEY[]{ L"RH:FILE_PATH" };
	constexpr wchar_t CHAPTER_INDEX_KEY[]{ L"RH:CHAPTER_INDEX" }; 

	constexpr wchar_t WINDOW_POSITION_X_KEY[]{ L"RH:WINDOW_POSITION_X" }; 
	constexpr wchar_t WINDOW_POSITION_Y_KEY[]{ L"RH:WINDOW_POSITION_Y" }; 

	constexpr wchar_t WINDOW_SIZE_W_KEY[]{ L"RH:WINDOW_SIZE_W" }; 
	constexpr wchar_t WINDOW_SIZE_H_KEY[]{ L"RH:WINDOW_SIZE_H" }; 
	


	ReadingHistory::ReadingHistory()
		: m_windowPositionX{}
		, m_windowPositionY{}
		, m_windowSizeW{}
		, m_windowSizeH{}
		, m_chapterIndex{}
	{
		auto appData = winrt::Microsoft::Windows::Storage::ApplicationData::GetDefault();
		m_localSettings = appData.LocalSettings().Values();
	}

	hstring ReadingHistory::FilePath() const
	{
		return m_filePath; 
	}
	void ReadingHistory::FilePath(hstring const& value)
	{
		m_filePath = value; 
	}

	uint32_t ReadingHistory::ChapterIndex() const
	{
		return m_chapterIndex; 
	}
	void ReadingHistory::ChapterIndex(uint32_t value)
	{
		m_chapterIndex = value; 
	}

	int32_t ReadingHistory::WindowPositionX() const
	{
		return m_windowPositionX; 
	}
	void ReadingHistory::WindowPositionX(int32_t value)
	{
		m_windowPositionX = value; 
	}

	int32_t ReadingHistory::WindowPositionY() const
	{
		return m_windowPositionY;
	}
	void ReadingHistory::WindowPositionY(int32_t value)
	{
		m_windowPositionY = value; 
	}

	int32_t ReadingHistory::WindowSizeW() const
	{
		return m_windowSizeW;
	}
	void ReadingHistory::WindowSizeW(int32_t value)
	{
		m_windowSizeW = value; 
	}

	int32_t ReadingHistory::WindowSizeH() const
	{
		return m_windowSizeH;
	}
	void ReadingHistory::WindowSizeH(int32_t value)
	{
		m_windowSizeH = value; 
	}

	bool ReadingHistory::Load()
	{
		if (!m_localSettings.HasKey(READING_HISTORY_KEY)) {
			return false; 
		}

		m_filePath = this->GetValue<winrt::hstring>(FILE_PATH_KEY); 
		m_chapterIndex = this->GetValue<uint32_t>(CHAPTER_INDEX_KEY); 
		m_windowPositionX = this->GetValue<int32_t>(WINDOW_POSITION_X_KEY); 
		m_windowPositionY = this->GetValue<int32_t>(WINDOW_POSITION_Y_KEY); 
		m_windowSizeW = this->GetValue<int32_t>(WINDOW_SIZE_W_KEY); 
		m_windowSizeH = this->GetValue<int32_t>(WINDOW_SIZE_H_KEY); 

		return true; 
	}

	void ReadingHistory::Save()
	{
		this->SaveValue<int32_t>(1, READING_HISTORY_KEY); 
		this->SaveValue<winrt::hstring>(m_filePath, FILE_PATH_KEY); 
		this->SaveValue<uint32_t>(m_chapterIndex, CHAPTER_INDEX_KEY); 

		this->SaveValue<int32_t>(m_windowPositionX, WINDOW_POSITION_X_KEY); 
		this->SaveValue<int32_t>(m_windowPositionY, WINDOW_POSITION_Y_KEY); 
		this->SaveValue<int32_t>(m_windowSizeW, WINDOW_SIZE_W_KEY);
		this->SaveValue<int32_t>(m_windowSizeH, WINDOW_SIZE_H_KEY);
	}
}
