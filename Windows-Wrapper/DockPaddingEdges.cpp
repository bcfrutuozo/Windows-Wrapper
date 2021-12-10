#include "DockPaddingEdges.h"
#include "ScrollableControl.h"

DockPaddingEdges::DockPaddingEdges(ScrollableControl* owner)
	:
	m_Owner(owner),
	m_Left(0),
	m_Right(0),
	m_Top(0),
	m_Bottom(0)
{}

DockPaddingEdges::DockPaddingEdges(int left, int right, int top, int bottom)
	:
	m_Owner(nullptr),
	m_Left(left),
	m_Right(right),
	m_Top(top),
	m_Bottom(bottom)
{}

void DockPaddingEdges::ResetAll()
{
	SetAll(0);
}

void DockPaddingEdges::ResetBottom()
{
	SetBottom(0);
}

void DockPaddingEdges::ResetLeft()
{
	SetLeft(0);
}

void DockPaddingEdges::ResetRight()
{
	SetRight(0);
}

void DockPaddingEdges::ResetTop()
{
	SetTop(0);
}

void DockPaddingEdges::Scale(float dx, float dy)
{
	assert(m_Owner != nullptr, "Owner cannot be null for padding scaling");
	m_Owner->GetPadding().Scale(dx, dy);
}

constexpr bool DockPaddingEdges::Equals(const Object* const other) const
{
	if(const DockPaddingEdges* p = dynamic_cast<const DockPaddingEdges*>(other))
	{
		Padding other = p->m_Owner->GetPadding();
		return m_Owner->GetPadding().Equals(&other);
	}

	return false;
}

int DockPaddingEdges::GetBottom() const noexcept
{
	if(m_Owner == nullptr)
	{
		return m_Bottom;
	}
	else
	{
		return m_Owner->GetPadding().Bottom;
	}
}

int DockPaddingEdges::GetLeft() const noexcept
{
	if(m_Owner == nullptr)
	{
		return m_Left;
	}
	else
	{
		return m_Owner->GetPadding().Left;
	}
}

int DockPaddingEdges::GetRight() const noexcept
{
	if(m_Owner == nullptr)
	{
		return m_Right;
	}
	else
	{
		return m_Owner->GetPadding().Right;
	}
}

int DockPaddingEdges::GetTop() const noexcept
{
	if(m_Owner == nullptr)
	{
		return m_Top;
	}
	else
	{
		return m_Owner->GetPadding().Top;
	}
}

void DockPaddingEdges::SetAll(int value)
{
	if(m_Owner == nullptr)
	{
		m_Left = value;
		m_Top = value;
		m_Right = value;
		m_Bottom = value;
	}
	else
	{
		m_Owner->SetPadding(Padding(value));
	}
}

void DockPaddingEdges::SetBottom(int value)
{
	if(m_Owner == nullptr)
	{
		m_Bottom = value;
	}
	else
	{
		Padding p = m_Owner->GetPadding();
		p.Bottom = value;
		m_Owner->SetPadding(p);
	}
}

void DockPaddingEdges::SetLeft(int value)
{
	if(m_Owner == nullptr)
	{
		m_Left = value;
	}
	else
	{
		Padding p = m_Owner->GetPadding();
		p.Left = value;
		m_Owner->SetPadding(p);
	}
}

void DockPaddingEdges::SetRight(int value)
{
	if(m_Owner == nullptr)
	{
		m_Right = value;
	}
	else
	{
		Padding p = m_Owner->GetPadding();
		p.Right = value;
		m_Owner->SetPadding(p);
	}
}

void DockPaddingEdges::SetTop(int value)
{
	if(m_Owner == nullptr)
	{
		m_Top = value;
	}
	else
	{
		Padding p = m_Owner->GetPadding();
		p.Top = value;
		m_Owner->SetPadding(p);
	}
}