#include "ScrollProperties.h"
#include "ScrollableControl.h"
#include "Exceptions.h"

#include <windows.h>

void ScrollProperties::UpdateScrollInfo()
{
	if(m_ParentControl->IsHandleCreated() && m_IsVisible)
	{
		SCROLLINFO si;
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_ALL;
		si.nMin = m_Minimum;
		si.nMax = m_Maximum;
		si.nPage = (m_ParentControl->HasAutoScroll()) ? GetPageSize() : m_LargeChange;
		si.nPos = m_Value;
		si.nTrackPos = 0;
		SetScrollInfo(m_ParentControl->GetHandle(), GetOrientation(), &si, true);
	}
}

void ScrollProperties::EnableScroll(bool enable)
{
	if(m_IsEnabled)
	{
		EnableScrollBar(m_ParentControl->GetHandle(), GetOrientation(), ESB_ENABLE_BOTH);
	}
	else
	{
		EnableScrollBar(m_ParentControl->GetHandle(), GetOrientation(), ESB_DISABLE_BOTH);
	}
}

ScrollProperties::ScrollProperties(ScrollableControl* const container)
	:
	m_Minimum(0),
	m_Maximum(100),
	m_SmallChange(1),
	m_LargeChange(10),
	m_Value(0),
	m_ParentControl(container),
	m_IsVisible(false),
	m_IsEnabled(true)
{}

ScrollableControl* ScrollProperties::GetParentControl() const noexcept
{
	return m_ParentControl;
}

int ScrollProperties::GetLargeChange() const noexcept
{
	return (std::min)(m_LargeChange, m_Maximum - m_Minimum + 1);
}

void ScrollProperties::SetLargeChange(int value)
{
	if(m_LargeChange != value)
	{
		if(value < 0) throw ArgumentOutOfRangeException("LargeChange", "InvalidLowBoundArgumentEx");

		m_LargeChange = value;
		m_LargeChangeSetExternally = true;
		UpdateScrollInfo();
	}
}

int ScrollProperties::GetMaximum() const noexcept
{
	return m_Maximum;
}

void ScrollProperties::SetMaximum(int value)
{
	if(m_ParentControl->HasAutoScroll()) return;

	if(m_Maximum != value)
	{
		if(m_Minimum > value)	m_Minimum = value;
		if(value < m_Value) m_Value = value;

		m_Maximum = value;
		m_MaximumSetExternally = true;
		UpdateScrollInfo();
	}
}

int ScrollProperties::GetMinimum() const noexcept
{
	return m_Minimum;
}

void ScrollProperties::SetMinimum(int value)
{
	if(m_ParentControl->HasAutoScroll()) return;

	if(m_Minimum != value)
	{
		if(value < 0) throw ArgumentOutOfRangeException("Minimum", "InvalidLowBoundArgumentEx");

		if(m_Maximum < value) m_Maximum = value;
		if(value > m_Value) m_Value = value;

		m_Minimum = value;
		UpdateScrollInfo();
	}
}

int ScrollProperties::GetValue() const noexcept
{
	return m_Value;
}

void ScrollProperties::SetValue(int value)
{
	if(m_Value != value)
	{
		if(value < m_Minimum || value < m_Maximum) throw ArgumentOutOfRangeException("Value", "InvalidLowBoundArgument");

		m_Value = value;
		UpdateScrollInfo();
		//m_ParentControl->SetDisplayFromScrollProps(GetHorizontalDisplayPosition(), GetVerticalDisplayPosition());
	}
}

int ScrollProperties::GetSmallChange() const noexcept
{
	return (std::min)(m_SmallChange, m_LargeChange);
}

void ScrollProperties::SetSmallChange(int value)
{
	if(m_SmallChange != value)
	{
		if(value < 0) throw ArgumentOutOfRangeException("SmallChange", "InvalidLowBoundArgumentEx");

		m_SmallChange = value;
		m_SmallChangeSetExternally = true;
		UpdateScrollInfo();
	}
}

bool ScrollProperties::IsEnabled() const noexcept
{
	return m_IsEnabled;
}

void ScrollProperties::Enable()
{
	if(m_ParentControl->HasAutoScroll()) return;

	m_IsEnabled = true;
	EnableScroll(true);
}

void ScrollProperties::Disable()
{
	if(m_ParentControl->HasAutoScroll()) return;

	m_IsEnabled = false;
	EnableScroll(false);
}

bool ScrollProperties::IsVisible() const noexcept
{
	return m_IsVisible;
}

void ScrollProperties::Show()
{
	if(!m_IsVisible)
	{
		m_IsVisible = true;
		//m_ParentControl->UpdateStyleCore();
		UpdateScrollInfo();
		//m_ParentControl->SetDisplayFromScrollProps(GetHorizontalDisplayPosition(), GetVerticalDisplayPosition());
	}
}

void ScrollProperties::Hide()
{
	if(m_IsVisible)
	{
		m_IsVisible = false;
		//m_ParentControl->UpdateStyleCore();
		UpdateScrollInfo();
		//m_ParentControl->SetDisplayFromScrollProps(GetHorizontalDisplayPosition(), GetVerticalDisplayPosition());
	}
}