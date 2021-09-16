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
	size_t m_ItemWidth;
	size_t m_ItemHeight;
	size_t m_MinimumItemWidth;
	RECT m_DrawableArea;

protected:

	HorizontalScrollBar HorizontalScrollBar;
	VerticalScrollBar VerticalScrollBar;

	void SetMinimumItemWidth(const size_t& weight) noexcept;
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
	size_t GetItemWidth() const noexcept;
	void SetItemWidth(const size_t& width);
	size_t GetItemHeight() const noexcept;
	void SetItemHeight(const size_t& height) noexcept;
	size_t GetHorizontalPage() const noexcept;
	size_t GetVerticalPage() const noexcept;
};

