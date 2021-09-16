#pragma once

#include "ScrollableControl.h"
#include "ListItemCollection.h"
#include "ListControlConvertEventHandler.h"

class ListControl : public ScrollableControl
{
private:

	bool m_AllowSelection;

	EventHandler* OnDataSourceChanged;
	EventHandler* OnDisplayMemberChanged;
	ListControlConvertEventHandler* OnFormat;
	EventHandler* OnFormatInfoChanged;
	EventHandler* OnFormatStringChanged;
	EventHandler* OnFormattingEnabledChanged;
	EventHandler* OnSelectedValueChanged;
	EventHandler* OnValueMemberChanged;

protected:

	int m_SelectedIndex;
	std::string m_SelectedValue;
	bool m_IsRebinding;
	ListItemCollection* Items;

	int OnEraseBackground_Impl(HWND hwnd, HDC hdc) noexcept override;

	ListControl(Control* parent, const std::string& name, int width, int x, int y);
	ListControl(Control* parent, const std::string& name, int width, int height, int x, int y);
	virtual ~ListControl();

public:

	void OnDataSourceChangedSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept;
	void OnDisplayMemberChangedSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept;
	void OnFormatSet(const std::function<void(Object*, ListControlConvertEventArgs*)>& callback) noexcept;
	void OnFormatInfoChangedSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept;
	void OnFormatStringChangedSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept;
	void OnFormattingEnabledChangedSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept;
	void OnSelectedValueChangedSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept;
	void OnValueMemberChangedSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept;

	void Initialize() override;
	bool IsSelectionAllowed();
	void EnableSelection() noexcept;
	void DisableSelection() noexcept;
	ListItemCollection* const GetDataSource() const noexcept;
	void SetDataSource(ListItemCollection* const dataSource) noexcept;
	virtual int GetSelectedIndex() const noexcept;
	virtual void SetSelectedIndex(int index) noexcept = 0;
	std::string GetSelectedValue() const noexcept;
	virtual void SetSelectedValue(const ListItem& item) = 0;
	bool IsRebinding() const noexcept;
};