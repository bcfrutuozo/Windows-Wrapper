#pragma once

#include "ListControl.h"

class ListBox : public ListControl
{
private:

	bool m_IsMultiColumn;
	bool m_IsHorizontalScrollVisible;
	bool m_IsScrollAlwaysVisible;
	bool m_IsFormatChanged;
	SelectionMode m_SelectionMode;
	DockStyle m_DockStyle;
	BorderStyle m_BorderStyle;
	std::vector<RECT> m_RowPosition;
	size_t m_TotalItemsInDrawableArea;
	size_t m_ColumnWidth;
	size_t m_RowNumber;
	size_t m_ColumnNumber;

	void OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept override;
	void OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept override;
	void OnPaint_Impl(HWND hwnd) noexcept override;

	void CalculateListBoxParameters(HWND hwnd, HDC& hdc);
	void Draw(HWND hwnd, HDC& hdc);

public:

	ListBox(Control* parent, int width, int height, int x, int y);
	virtual ~ListBox();

	bool IsMultiColumn() const noexcept;
	void EnableMultiColumn() noexcept;
	void DisableMultiColumn() noexcept;
	size_t GetColumnWidth() noexcept;
	void SetColumnWidth(const size_t& width) noexcept;
	BorderStyle GetBorderStyle() const noexcept;
	void SetBorderStyle(BorderStyle style) noexcept;
};