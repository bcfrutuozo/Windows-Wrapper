#pragma once

#include "Control.h"

class Label final : public Control
{
private:

	BorderStyle m_BorderStyle;
	FlatStyle m_FlatStyle;

	virtual void OnPaint_Impl(HWND hwnd) noexcept override;

public:

	Label(Control* parent, const std::string& text, int x, int y);
	virtual ~Label() = default;

	void Initialize() override;
	BorderStyle GetBorderStyle() const noexcept;
	void SetBorderStyle(BorderStyle style) noexcept;
	FlatStyle GetFlatStyle() const noexcept;
	void SetFlatStyle(FlatStyle style) noexcept;
};

