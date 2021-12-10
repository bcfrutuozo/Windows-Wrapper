#include "ContainerControl.h"
#include "Exceptions.h"

bool ContainerControl::ActivateControlInternal(Control* control, bool originator)
{
	return false;
}

ContainerControl::ContainerControl()
	:
	ScrollableControl(),
	m_AutoScaleDimensions(0 ,0),
	m_CurrentAutoScaleDimensions(0, 0)
{
	SetStyle(ControlStyles::AllPaintingInWmPaint, false);
}

bool ContainerControl::ActivateControl(Control* const control)
{
	return ActivateControlInternal(control, true);
}

Control* ContainerControl::GetActiveControl() const noexcept
{
	return m_ActiveControl;
}

void ContainerControl::SetActiveControl(Control* const control)
{
	if(m_ActiveControl != control || (control != nullptr && !control->IsFocused()))
	{
		if(control != nullptr && !Contains(control)) throw ArgumentException("CannotActivateControl");

		ContainerControl* cc = this;

		if(control != nullptr && control->GetParent() != nullptr)
		{
			cc = (ContainerControl*)control->GetParent()->GetContainerControl();
		}
	}
}
