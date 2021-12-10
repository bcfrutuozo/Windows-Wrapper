#pragma once

#include "Object.h"

class Control;

class IContainerControl : public Object
{
public:

	virtual bool ActivateControl(Control* const control) = 0;
	virtual Control* GetActiveControl() const noexcept = 0;
	virtual void SetActiveControl(Control* const control) = 0;
};