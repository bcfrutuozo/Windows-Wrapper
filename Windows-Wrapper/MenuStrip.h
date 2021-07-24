#pragma once

#include "MenuRoot.h"

class Window;

class MenuStrip : public MenuRoot
{
	friend class Window;

public:

	MenuStrip(Control* parent);
	virtual ~MenuStrip() = default;
};