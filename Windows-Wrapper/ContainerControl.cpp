#include "ContainerControl.h"
#include "Window.h"
#include "Exceptions.h"
#include "UserControl.h"
#include "UnsafeNativeMethods.h"

using namespace Microsoft::Win32;


bool ContainerControl::ActivateControlInternal(Control* control)
{
	return ActivateControlInternal(control, true);
}

bool ContainerControl::ActivateControlInternal(Control* control, bool originator)
{
	bool ret = true;
	bool updateContainerActiveControl = false;
	ContainerControl* cc = nullptr;
	Control* parent = Parent;
	if(parent != nullptr)
	{
		cc = (ContainerControl*)parent->GetContainerControl();
		if(cc != nullptr)
		{
			updateContainerActiveControl = cc->GetActiveControl() != this;
		}
	}

	if(control != m_ActiveControl || updateContainerActiveControl)
	{
		if(updateContainerActiveControl)
		{
			if(!cc->ActivateControlInternal(this, false)) return false;
		}

		ret = AssignActiveControlInternal((control == this) ? nullptr : control);
	}

	if(originator) ScrollActiveControlIntoView();

	return ret;
}

bool ContainerControl::AssignActiveControlInternal(Control* value)
{
	if(m_ActiveControl != value)
	{
		m_ActiveControl = value;
		UpdateFocusedControl();

		Window* wnd = FindWindowInternal();
		if(wnd != nullptr) wnd->UpdateDefaultButton();
	}
	else
	{
		m_FocusedControl = m_ActiveControl;
	}

	return (m_ActiveControl == value);
}

void ContainerControl::EnsureUnvalidatedControl(Control* candidate)
{
	m_UnvalidatedControl = candidate;

	while(const auto container = (ContainerControl*)m_UnvalidatedControl)
	{
		if(container->m_UnvalidatedControl != nullptr && container->m_UnvalidatedControl->ShouldAutoValidate())
		{
			m_UnvalidatedControl = container->m_UnvalidatedControl;
		}
		else if(container->m_ActiveControl != nullptr && container->m_ActiveControl->ShouldAutoValidate())
		{
			m_UnvalidatedControl = container->m_ActiveControl;
		}
		else
		{
			break;
		}
	}
}

void ContainerControl::EnterValidation(Control* enterControl)
{
	if(m_UnvalidatedControl == nullptr) return;
	if(!enterControl->CausesValidation()) return;

	AutoValidate autoValidateMode = Control::GetAutoValidateForControl(m_UnvalidatedControl);
	if(autoValidateMode == AutoValidate::Disable) return;

	Control* commonAncestor = enterControl;
	while(commonAncestor != nullptr && !commonAncestor->IsDescendant(m_UnvalidatedControl))
	{
		commonAncestor = commonAncestor->Parent;
	}

	bool preventFocusChangeOnError = (autoValidateMode == AutoValidate::EnablePreventFocusChange);
	ValidateThroughAncestor(commonAncestor, preventFocusChangeOnError);
}

ScrollableControl* ContainerControl::FindScrollableParent(Control* control)
{
	Control* current = control->Parent;
	while(current != nullptr && !(ScrollableControl*)current)
	{
		current = current->Parent;
	}

	if(current != nullptr) return (ScrollableControl*)current;

	return nullptr;
}

ContainerControl* ContainerControl::GetInnerMostActiveControl()
{
	ContainerControl* ret = this;
	while(const auto c = (ContainerControl*)ret->m_ActiveControl)
	{
		ret = c;
	}
	return ret;
}

ContainerControl* ContainerControl::GetInnerMostFocusedControl()
{
	ContainerControl* ret = this;
	while(const auto c = (ContainerControl*)ret->m_FocusedControl)
	{
		ret = c;
	}
	return ret;
}

void ContainerControl::FocusActiveControlInternal()
{
	if(m_ActiveControl != nullptr && m_ActiveControl->IsVisible())
	{
		IntPtr focusHandle = GetFocus();
		if(focusHandle == IntPtr::Zero() || Control::FromChildHandleInternal(focusHandle) != m_ActiveControl)
		{
			UnsafeNativeMethods::SetFocus(m_ActiveControl->GetHandle());
		}
	}
	else
	{
		ContainerControl* cc = this;
		while(cc != nullptr && !cc->IsVisible())
		{
			Control* parent = cc->Parent;
			if(parent != nullptr)
			{
				cc = (ContainerControl*)parent->GetContainerControl();
			}
			else
			{
				break;
			}
		}

		if(cc != nullptr && cc->IsVisible())
		{
			UnsafeNativeMethods::SetFocus(cc->GetHandle());
		}
	}
}

void ContainerControl::ResetActiveAndFocusedControlsRecursive()
{
	if(const auto c = (ContainerControl*)m_ActiveControl)
	{
		c->ResetActiveAndFocusedControlsRecursive();
	}

	m_ActiveControl = nullptr;
	m_FocusedControl = nullptr;
}

void ContainerControl::ResetValidationFlag()
{
	for(const auto& c : Controls)
	{
		c->SetValidationCancelled(true);
	}
}

void ContainerControl::ScrollActiveControlIntoView()
{
	Control* last = m_ActiveControl;
	if(last != nullptr)
	{
		ScrollableControl* scrollParent = FindScrollableParent(last);

		while(scrollParent != nullptr)
		{
			scrollParent->ScrollControlIntoView(m_ActiveControl);
			last = scrollParent;
			scrollParent = FindScrollableParent(scrollParent);
		}
	}
}

void ContainerControl::UpdateFocusedControl()
{
	EnsureUnvalidatedControl(m_FocusedControl);
	Control* pathControl = m_FocusedControl;

	while(m_ActiveControl != pathControl)
	{
		if(pathControl == nullptr || pathControl->IsDescendant(m_ActiveControl))
		{
			Control* nextControlDown = m_ActiveControl;
			while(true)
			{
				Control* parent = nextControlDown->Parent;
				if(parent == this || parent == pathControl) break;

				nextControlDown = nextControlDown->Parent;
			}

			Control* priorFocusedControl = m_FocusedControl = pathControl;
			EnterValidation(nextControlDown);
			if(m_FocusedControl != priorFocusedControl)
			{
				pathControl = m_FocusedControl;
				continue;
			}

			pathControl = nextControlDown;

			pathControl->NotifyEnter();
		}
		else
		{
			ContainerControl* innerMostFCC = GetInnerMostFocusedControl();
			Control* stopControl = nullptr;

			if(innerMostFCC->m_FocusedControl != nullptr)
			{
				pathControl = innerMostFCC->m_FocusedControl;
				stopControl = innerMostFCC;

				if(innerMostFCC != this)
				{
					innerMostFCC->m_FocusedControl = nullptr;
					if(!(innerMostFCC->Parent != nullptr) && (MDIClient*)innerMostFCC->Parent)
					{
						innerMostFCC->m_ActiveControl = nullptr;
					}
				}
			}
			else
			{
				pathControl = innerMostFCC;
				if(innerMostFCC->Parent != nullptr)
				{
					ContainerControl* cc = (ContainerControl*)(innerMostFCC->Parent->GetContainerControl());
					stopControl = cc;
					if(cc != nullptr && cc != this)
					{
						cc->m_FocusedControl = nullptr;
						cc->m_ActiveControl = nullptr;
					}
				}
			}

			do
			{
				Control* leaveControl = pathControl;

				if(pathControl != nullptr) pathControl = pathControl->Parent;
				if(pathControl == this) pathControl = nullptr;

				if(leaveControl != nullptr)
				{
					leaveControl->NotifyLeave();
				}

			} while(pathControl != nullptr && pathControl != stopControl && !pathControl->IsDescendant(m_ActiveControl));
		}
	}

	m_FocusedControl = m_ActiveControl;
	if(m_ActiveControl != nullptr) EnterValidation(m_ActiveControl);
}

bool ContainerControl::ValidateThroughAncestor(Control* ancestorControl, bool preventFocusChangeOnError)
{
	if(ancestorControl == nullptr) ancestorControl = this;
	if(m_UnvalidatedControl == nullptr) m_UnvalidatedControl = m_FocusedControl;
	if(m_UnvalidatedControl == nullptr) return true;
	if(!ancestorControl->IsDescendant(m_UnvalidatedControl)) return false;

	bool cancel = false;

	Control* currentActiveControl = m_ActiveControl;
	Control* currentValidatingControl = m_UnvalidatedControl;
	if(currentActiveControl != nullptr)
	{
		currentActiveControl->SetValidationCancelled(false);
		if(const auto currentActiveContainerControl = (ContainerControl*)currentActiveControl)
		{
			currentActiveContainerControl->ResetValidationFlag();
		}
	}

	while(currentValidatingControl != nullptr && currentValidatingControl != ancestorControl)
	{
		try
		{
			cancel = currentValidatingControl->PerformControlValidation(false);
		}
		catch(...)
		{
			cancel = true;
			throw;
		}

		if(cancel)
		{
			break;
		}

		currentValidatingControl = currentValidatingControl->Parent;
	}

	if(cancel && preventFocusChangeOnError)
	{
		if(m_UnvalidatedControl == nullptr && currentValidatingControl != nullptr && ancestorControl->IsDescendant(currentValidatingControl))
		{
			m_UnvalidatedControl = currentValidatingControl;
		}
		// This bit 'marks' the control that was going to get the focus, so that it will ignore any pending
		// mouse or key events. Otherwise it would still perform its default 'click' action or whatever.
		if(currentActiveControl == m_ActiveControl)
		{
			if(currentActiveControl != nullptr)
			{
				CancelEventArgs* ev = new CancelEventArgs();
				ev->Cancel = true;
				currentActiveControl->NotifyValidationResult(currentValidatingControl, ev);
				delete ev;

				if(const auto currentActiveContainerControl = (ContainerControl*)currentActiveControl)
				{
					if(currentActiveContainerControl->m_FocusedControl != nullptr)
					{
						currentActiveContainerControl->m_FocusedControl->SetValidationCancelled(true);
					}

					currentActiveContainerControl->ResetActiveAndFocusedControlsRecursive();
				}
			}
		}
		// This bit forces the focus to move back to the invalid control
		SetActiveControlInternal(m_UnvalidatedControl);
	}

	m_UnvalidatedControl = nullptr;

	return !cancel;
}

bool ContainerControl::HasFocusableChild()
{
	Control* ctl = nullptr;
	do
	{
		ctl = GetNextControl(ctl, true);
		if(ctl != nullptr && ctl->CanSelect() && ctl->TabStop())
		{
			break;
		}

	} while(ctl != nullptr);

	return ctl != nullptr;
}

void ContainerControl::SetActiveControlInternal(Control* value)
{
	if(m_ActiveControl != value || (value != nullptr && !value->IsFocused()))
	{
		if(value != nullptr && !Contains(value)) throw ArgumentException("CannotActivateControl");

		bool ret;
		ContainerControl* cc = this;

		if(value != nullptr && value->Parent != nullptr)
		{
			cc = (ContainerControl*)value->Parent->GetContainerControl();
		}

		if(cc != nullptr)
		{
			ret = cc->ActivateControlInternal(value, false);
		}
		else
		{
			ret = AssignActiveControlInternal(value);
		}

		if(cc != nullptr && &ret)
		{
			ContainerControl* ccAncestor = this;
			while(ccAncestor->Parent != nullptr &&
				  (ContainerControl*)ccAncestor->Parent->GetContainerControl())
			{
				ccAncestor = (ContainerControl*)ccAncestor->Parent->GetContainerControl();
				assert(ccAncestor != nullptr);
			}

			if(ccAncestor->ContainsFocus() &&
			   (value == nullptr ||
				!(UserControl*)value ||
				((UserControl*)value && !((UserControl*)value)->HasFocusableChild())))\
			{
				cc->FocusActiveControlInternal();
			}
		}
	}
}

void ContainerControl::OnAutoValidateChanged(EventArgs* e)
{
	Dispatch("OnAutoValidateChanged", e);
}

void ContainerControl::UpdateDefaultButton()
{
	// Hook for form
}

void ContainerControl::WmSetFocus(Message& m)
{
	if(IsHostedInWin32DialogManager())
	{
		if(GetActiveControl() != nullptr)
		{
			//WmImeSetFocus();
			if(!GetActiveControl()->IsVisible()) InvokeGotFocus(this, &ArgsDefault);
			FocusActiveControlInternal();
		}
		else
		{
			if(Parent != nullptr)
			{
				IContainerControl* c = Parent->GetContainerControl();
				if(c != nullptr)
				{
					bool succeded = false;
					ContainerControl* knowContainer = (ContainerControl*)c;
					if(knowContainer != nullptr)
					{
						succeded = knowContainer->ActivateControlInternal(this);
					}
					else
					{
						succeded = c->ActivateControl(this);
					}

					if(!succeded) return;
				}
			}
			ScrollableControl::WndProc(m);
		}
	}
	else
	{
		ScrollableControl::WndProc(m);
	}
}

void ContainerControl::WndProc(Message& m)
{
	switch(m.Msg)
	{
		case WM_SETFOCUS: WmSetFocus(m); break;
		default: ScrollableControl::WndProc(m); break;
	}
}

ContainerControl::ContainerControl()
	:
	ScrollableControl(),
	m_ActiveControl(nullptr),
	m_FocusedControl(nullptr),
	m_UnvalidatedControl(nullptr),
	m_AutoScaleDimensions(0, 0),
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

AutoValidate ContainerControl::GetAutoValidate()
{
	if(m_AutoValidate == AutoValidate::Inherit) return GetAutoValidateForControl(this);
	else m_AutoValidate;
}

void ContainerControl::SetAutoValidate(AutoValidate value)
{
	switch(value)
	{
		case AutoValidate::Disable:
		case AutoValidate::EnablePreventFocusChange:
		case AutoValidate::EnableAllowFocusChange:
		case AutoValidate::Inherit:
			break;
		default:
			throw InvalidEnumArgumentException("AutoValidate", (int)value, "AutoValidate");
	}

	if(m_AutoValidate != value)
	{
		m_AutoValidate = value;
		OnAutoValidateChanged(&ArgsDefault);
	}
}
