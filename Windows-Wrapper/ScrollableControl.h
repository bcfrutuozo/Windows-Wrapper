#pragma once

#include "Control.h"
#include "HorizontalScrollBar.h"
#include "VerticalScrollBar.h"

class ScrollableControl : public Control
{
	friend class ScrollBar;
	friend class HorizontalScrollBar;
	friend class VerticalScrollBar;

private:

	bool m_IsHorizontalScrollEnabled;
	bool m_IsVerticalScrollEnabled;
	int m_ItemWidth;
	int m_ItemHeight;
	int m_MinimumItemWidth;
	RECT m_DrawableArea;

protected:

	HorizontalScrollBar HorizontalScrollBar;
	VerticalScrollBar VerticalScrollBar;

	void SetMinimumItemWidth(const int& weight) noexcept;
	LPRECT const GetDrawableArea() noexcept;
	LPRECT const ResetDrawableArea() noexcept;

	ScrollableControl(Control* parent, const std::string& name, int width, int height, int x, int y);
	virtual ~ScrollableControl();

public:

	bool IsHorizontalScrollEnabled() const noexcept;
	void EnableHorizontalScroll() noexcept;
	void DisableHorizontalScroll() noexcept;
	bool IsVerticalScrollEnabled() const noexcept;
	void EnableVerticalScroll() noexcept;
	void DisableVerticalScroll() noexcept;
	int GetItemWidth() const noexcept;
	void SetItemWidth(const int& width);
	int GetItemHeight() const noexcept;
	void SetItemHeight(const int& height) noexcept;
	int GetHorizontalPage() const noexcept;
	int GetVerticalPage() const noexcept;
};

