#pragma once

#include "Control.h"

class Label final : public Control
{
	friend class Control;

private:

	BorderStyle m_BorderStyle;
	FlatStyle m_FlatStyle;

	void Draw(Graphics* const graphics, Drawing::Rectangle rectangle) override;

	Label(Control* parent, const std::string& text, int x, int y);

public:

	virtual ~Label();

	void Initialize() override;
	BorderStyle GetBorderStyle() const noexcept;
	void SetBorderStyle(BorderStyle style) noexcept;
	FlatStyle GetFlatStyle() const noexcept;
	void SetFlatStyle(FlatStyle style) noexcept;
};

