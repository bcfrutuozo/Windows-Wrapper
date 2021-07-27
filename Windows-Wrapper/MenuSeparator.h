#pragma once

#include "Menu.h"

class MenuSeparator : public Menu
{
protected:

	void Initialize() noexcept override;

public:

	MenuSeparator(Menu* parent, unsigned int subitemIndex);
	virtual ~MenuSeparator() = default;
};