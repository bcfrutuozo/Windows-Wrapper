#pragma once

#include "Control.h"
#include "FlatButtonAppearance.h"
#include "Image.h"

class Button final: public Control
{
	friend class Control;

private:

	//Image m_BackgroundImage;
	FlatButtonAppearance m_FlatAppearance;
	FlatStyle m_FlatStyle;

	void Draw(Graphics* const graphics, Drawing::Rectangle rectangle) override;
	int OnEraseBackground_Impl(HWND hwnd, HDC hdc) override;
	void OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) override;
	void OnKeyUp_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) override;

	Button(Control* parent, const std::string& name, int width, int height, int x, int y);

public:

	virtual ~Button();

	void Initialize() override;
	FlatButtonAppearance GetFlatButtonAppearance() const noexcept;
	void SetFlatButtonAppearance(FlatButtonAppearance appearance) noexcept;
	FlatStyle GetFlatStyle() const noexcept;
	void SetFlatStyle(FlatStyle style) noexcept;
};

