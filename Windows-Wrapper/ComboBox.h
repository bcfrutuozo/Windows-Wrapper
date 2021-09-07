#pragma once

#include "ListControl.h"

class ComboBox : public ListControl
{
private:

	FlatStyle m_FlatStyle;

	void OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept override;
	void OnPaint_Impl(HWND hwnd) noexcept override;

public:

	ComboBox(Control* parent, const std::string& name, int width, int x, int y);
	virtual ~ComboBox();

	void IncrementHorizontalScroll() noexcept override;
	void DecrementHorizontalScroll() noexcept override;
	void IncrementVerticalScroll() noexcept override;
	void DecrementVerticalScroll() noexcept override;
};

