#pragma once

#include "ListControl.h"

class ComboBox;

class ListBox : public ListControl
{
	friend class ComboBox;
	friend class Control;

private:

	bool m_IsMultiColumn;
	bool m_IsHorizontalScrollVisible;
	bool m_IsScrollAlwaysVisible;
	bool m_IsFormatChanged;
	SelectionMode m_SelectionMode;
	DockStyle m_DockStyle;
	BorderStyle m_BorderStyle;
	std::vector<RECT> m_RowPosition;
	int m_TotalItemsInDrawableArea;
	int m_ColumnWidth;
	int m_RowNumber;
	int m_ColumnNumber;
	ListItem* m_SelectedItem;
	std::vector<int> m_SelectedIndices;
	std::vector<ListItem*> m_SelectedItems;
	int m_Tabulation;

	void PreDraw(const Graphics& graphics) override;
	void Draw(const Graphics& graphics, Drawing::Rectangle rectangle) override;
	void OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept override;
	void OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept override;

	ListBox(Control* parent, int width, int height, int x, int y);


public:

	virtual ~ListBox();

	void SetSelectedIndex(int index) override;
	void SetSelectedValue(const ListItem& item) override;

	bool IsMultiColumn() const noexcept;
	void EnableMultiColumn() noexcept;
	void DisableMultiColumn() noexcept;
	int GetColumnWidth() noexcept;
	void SetColumnWidth(const int& width) noexcept;
	BorderStyle GetBorderStyle() const noexcept;
	void SetBorderStyle(BorderStyle style) noexcept;
	SelectionMode GetSelectionMode() const noexcept;
	void SetSelectionMode(SelectionMode mode) noexcept;
	void SelectAll();
	void ClearSelected() noexcept;
};