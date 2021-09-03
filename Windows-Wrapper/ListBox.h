#pragma once

#include "ListControl.h"

class ListBox : public ListControl
{
private:

	bool m_IsMultiColumn;
	bool m_IsHorizontalScrollVisible;
	bool m_IsVerticalScrollVisible;
	bool m_IsScrollAlwaysVisible;
	SelectionMode m_SelectionMode;
	DockStyle m_DockStyle;
	BorderStyle m_BorderStyle;
	std::vector<RECT> m_RowPosition;
	SIZE m_SingleSize;

	void OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept override;
	void OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept override;
	void OnPaint_Impl(HWND hwnd) noexcept override;

public:

	ListBox(Control* parent, int width, int height, int x, int y);
	virtual ~ListBox();
};