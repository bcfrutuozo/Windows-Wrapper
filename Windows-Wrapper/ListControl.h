#pragma once

#include "Control.h"
#include "IList.h"
#include "ListControlConvertEventHandler.h"

class ListControl : public Control
{
private:

	Object* m_DataSource;
	bool m_AllowSelection;
	unsigned int m_SelectedIndex;
	Object* m_SelectedValue;

	EventHandler* OnDataSourceChanged;
	EventHandler* OnDisplayMemberChanged;
	ListControlConvertEventHandler* OnFormat;
	EventHandler* OnFormatInfoChanged;
	EventHandler* OnFormatStringChanged;
	EventHandler* OnFormattingEnabledChanged;
	EventHandler* OnSelectedValueChanged;
	EventHandler* OnValueMemberChanged;

	// Singleton manages registration/cleanup of window class
	class ListClass
	{
	private:

		static constexpr const char* m_ClassName = "List Class";
		static ListClass m_ListClass;
		HINSTANCE m_Instance;

		ListClass() noexcept;
		~ListClass() noexcept;
		ListClass(const ListClass&) = delete;
		ListClass& operator=(const ListClass&) = delete;

	public:

		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	};

	int OnEraseBackground_Impl(HWND hwnd, HDC hdc) noexcept override;

public:

	ListControl();
	virtual ~ListControl();

	void OnDataSourceChangedSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept;
	void OnDisplayMemberChangedSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept;
	void OnFormatSet(const std::function<void(Object*, ListControlConvertEventArgs*)>& callback) noexcept;
	void OnFormatInfoChangedSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept;
	void OnFormatStringChangedSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept;
	void OnFormattingEnabledChangedSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept;
	void OnSelectedValueChangedSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept;
	void OnValueMemberChangedSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept;

	bool IsSelectionAllowed();
	void EnableSelection() noexcept;
	void DisableSelection() noexcept;
	void SetDataSource(Object* dataSource) noexcept;
	unsigned int GetSelectedIndex() const noexcept;
	void SetSelectedIndex(unsigned int index) noexcept;
	Object* GetSelectedValue() const noexcept;
};

