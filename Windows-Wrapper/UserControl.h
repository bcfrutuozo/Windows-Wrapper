#pragma once

#include "ContainerControl.h"
#include "BorderStyle.h"
#include "Size.h"

class UserControl : public ContainerControl
{
private:

	BorderStyle m_BorderStyle;

protected:

	CreateParams* CreateParameters() override;

	virtual Size DefaultSize() override { return Size((150, 150)); }

public:

	UserControl();
};