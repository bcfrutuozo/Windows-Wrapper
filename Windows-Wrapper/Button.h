#pragma once

#include "Control.h"
#include "IActivable.h"
#include "IHidable.h"
#include "IWinControl.h"

class Button : public Control, public IHidable, public IActivable
{
public:

	Button(Control* parent, const std::string& name, int width, int height, int x, int y);
	virtual ~Button();

	void Disable() noexcept override;
	void Enable() noexcept override;
	void Hide() override;
	void Initialize() noexcept override;
	void Show() override;
};

