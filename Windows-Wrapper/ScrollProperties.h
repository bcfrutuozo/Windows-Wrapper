#pragma once

#include "Object.h"

class ScrollableControl;

class ScrollProperties : public Object
{
private:

	int m_Minimum;
	int m_Maximum;
	int m_SmallChange;
	int m_LargeChange;
	int m_Value;
	bool m_MaximumSetExternally;
	bool m_SmallChangeSetExternally;
	bool m_LargeChangeSetExternally;


	ScrollableControl* m_ParentControl;
	bool m_IsVisible;
	bool m_IsEnabled;
	static constexpr int SCROLL_LINE = 5;

	void UpdateScrollInfo();
	void EnableScroll(bool enable);

protected:

	ScrollProperties(ScrollableControl* const container);

	ScrollableControl* GetParentControl() const noexcept;

public:

	int GetLargeChange() const noexcept;
	void SetLargeChange(int value);
	int GetMaximum() const noexcept;
	void SetMaximum(int value);
	int GetMinimum() const noexcept;
	void SetMinimum(int value);
	int GetValue() const noexcept;
	void SetValue(int value);
	int GetSmallChange() const noexcept;
	void SetSmallChange(int value);
	bool IsEnabled() const noexcept;
	void Enable();
	void Disable();
	bool IsVisible() const noexcept;
	void Show();
	void Hide();

	virtual int GetPageSize() const noexcept = 0;
	virtual int GetOrientation() const noexcept = 0;
	virtual int GetHorizontalDisplayPosition() const noexcept = 0;
	virtual int GetVerticalDisplayPosition() const noexcept = 0;
};