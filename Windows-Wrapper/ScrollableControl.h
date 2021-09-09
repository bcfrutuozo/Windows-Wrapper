#pragma once

#include "Control.h"
#include "HorizontalScrollBar.h"
#include "VerticalScrollBar.h"

class ScrollableControl : public Control
{
private:

	bool m_IsHorizontalScrollEnabled;
	bool m_IsVerticalScrollEnabled;
	size_t m_ItemWidth;
	size_t m_ItemHeight;

protected:

	HorizontalScrollBar HorizontalScrollBar;
	VerticalScrollBar VerticalScrollBar;

public:

	ScrollableControl(Control* parent, const std::string& name, int width, int height, int x, int y);
	virtual ~ScrollableControl();

	bool IsHorizontalScrollEnabled() const noexcept;
	void EnableHorizontalScroll() noexcept;
	void DisableHorizontalScroll() noexcept;
	bool IsVerticalScrollEnabled() const noexcept;
	void EnableVerticalScroll() noexcept;
	void DisableVerticalScroll() noexcept;
	size_t GetItemWidth() const noexcept;
	void SetItemWidth(const size_t& width) noexcept;
	size_t GetItemHeight() const noexcept;
	void SetItemHeight(const size_t& height) noexcept;

	virtual void IncrementHorizontalScroll() noexcept = 0;
	virtual void DecrementHorizontalScroll() noexcept = 0;
	virtual void IncrementVerticalScroll() noexcept = 0;
	virtual void DecrementVerticalScroll() noexcept = 0;
};

