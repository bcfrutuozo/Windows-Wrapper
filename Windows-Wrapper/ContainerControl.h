#pragma once

#include "ScrollableControl.h"
#include "IContainerControl.h"
#include "AutoValidate.h"
#include "AutoScaleMode.h"

class Window;

class ContainerControl : public ScrollableControl, public IContainerControl
{
	friend class Window;
	
private:

	Control* m_ActiveControl;
	Control* m_FocusedControl;
	Control* m_UnvalidatedControl;
	AutoValidate m_AutoValidate;

	SizeF m_AutoScaleDimensions;
	SizeF m_CurrentAutoScaleDimensions;
	AutoScaleMode m_AutoScaleMode;

	bool ActivateControlInternal(Control* control);
	bool ActivateControlInternal(Control* control, bool originator);
	bool AssignActiveControlInternal(Control* value);
	void EnsureUnvalidatedControl(Control* candidate);
	void EnterValidation(Control* enterControl);
	ScrollableControl* FindScrollableParent(Control* control);
	void ResetActiveAndFocusedControlsRecursive();
	void ResetValidationFlag();
	void ScrollActiveControlIntoView();
	void UpdateFocusedControl();
	bool ValidateThroughAncestor(Control* ancestorControl, bool preventFocusChangeOnError);

protected:

	void FocusActiveControlInternal();
	bool HasFocusableChild();
	void SetActiveControlInternal(Control* value);
	virtual void OnAutoValidateChanged(EventArgs* e);
	virtual void UpdateDefaultButton();
	ContainerControl* GetInnerMostActiveControl();
	ContainerControl* GetInnerMostFocusedControl();

	void WmSetFocus(Message& m);
	void WndProc(Message& m) override;

public:

	ContainerControl();

	virtual bool ActivateControl(Control* const control) override;
	virtual Control* GetActiveControl() const noexcept override;
	virtual void SetActiveControl(Control* const control) override;
	virtual AutoValidate GetAutoValidate();
	virtual void SetAutoValidate(AutoValidate value);
};