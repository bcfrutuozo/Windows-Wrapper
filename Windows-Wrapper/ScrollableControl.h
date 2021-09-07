#pragma once

#include "Control.h"
#include "HorizontalScrollBar.h"
#include "VerticalScrollBar.h"

class ScrollableControl : public Control
{
private:

	bool m_IsHorizontalScrollEnabled;
	bool m_IsVerticalScrollEnabled;
	size_t m_SingleHorizontalIncrement;
	size_t m_SingleVerticalIncrement;

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
	size_t GetSingleHorizontalIncrement() const noexcept;
	void SetSingleHorizontalIncrement(const size_t& increment) noexcept;
	size_t GetSingleVerticalIncrement() const noexcept;
	void SetSingleVerticalIncrement(const size_t& increment) noexcept;

	virtual void IncrementHorizontalScroll() noexcept = 0;
	virtual void DecrementHorizontalScroll() noexcept = 0;
	virtual void IncrementVerticalScroll() noexcept = 0;
	virtual void DecrementVerticalScroll() noexcept = 0;
};

