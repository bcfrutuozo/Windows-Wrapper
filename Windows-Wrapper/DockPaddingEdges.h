#pragma once

#include "Object.h"

class ScrollableControl;

class DockPaddingEdges : public Object
{
	friend class ScrollableControl;

private:

	ScrollableControl* m_Owner;
	int m_Left;
	int m_Right;
	int m_Top;
	int m_Bottom;

	DockPaddingEdges(ScrollableControl* owner);
	DockPaddingEdges(int left, int right, int top, int bottom);

	void ResetAll();
	void ResetBottom();
	void ResetLeft();
	void ResetRight();
	void ResetTop();
	void Scale(float dx, float dy);

public:

	constexpr bool Equals(const Object* const other) const override;
	int GetBottom() const noexcept;
	int GetLeft() const noexcept;
	int GetRight() const noexcept;
	int GetTop() const noexcept;
	void SetAll(int value);
	void SetBottom(int value);
	void SetLeft(int value);
	void SetRight(int value);
	void SetTop(int value);
};