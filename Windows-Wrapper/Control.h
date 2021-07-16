#pragma once

#include "Base.h"
#include "Common.h"
#include "IHandle.h"
#include "Size.h"
#include "Event.h"

class Control : public IHandle, public Base
{
protected:

	std::string Text;
	Size Size;
	Point Location;

public:

	Control* Parent;

	Control();
	Control(Control* parent, const std::string& text);
	Control(Control* parent, const std::string& text, int width, int height, int x, int y);
	Control(const std::string& text);
	Control(const std::string& text, int width, int height, int x, int y);
	~Control();

	void operator() (Control* p) {
		delete p;
	}

	const std::string& GetText() noexcept
	{
		return Text;
	}

	virtual void SetText(const std::string& title) = 0;
};

