#pragma once

#include "ScrollableControl.h"
#include "IContainerControl.h"
#include "AutoValidate.h"
#include "AutoScaleMode.h"

class ContainerControl : public ScrollableControl, public IContainerControl
{
private:

	Control* m_ActiveControl;
	Control* m_FocusedControl;
	Control* m_UnvalidatedControl;
	AutoValidate m_AutoValidate;

	SizeF m_AutoScaleDimensions;
	SizeF m_CurrentAutoScaleDimensions;
	AutoScaleMode m_AutoScaleMode;

	bool ActivateControlInternal(Control* control, bool originator);

public:

	ContainerControl();

	virtual bool ActivateControl(Control* const control) override;
	virtual Control* GetActiveControl() const noexcept override;
	virtual void SetActiveControl(Control* const control) override;
};