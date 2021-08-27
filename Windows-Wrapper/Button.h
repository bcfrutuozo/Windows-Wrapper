#pragma once

#include "IHidable.h"
#include "Control.h"

class Button final: public Control, public IHidable
{
	friend class WinAPI;

private:

	// Singleton manages registration/cleanup of window class
	class ButtonClass
	{
	private:

		static constexpr const char* m_ClassName = "Button Class";
		static ButtonClass m_ButtonClass;
		HINSTANCE m_Instance;

		ButtonClass() noexcept;
		~ButtonClass() noexcept;
		ButtonClass(const ButtonClass&) = delete;
		ButtonClass& operator=(const ButtonClass&) = delete;

	public:

		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	};

	int OnEraseBackground_Impl(HWND hwnd, HDC hdc) noexcept override;
	void OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept override;
	void OnKeyUp_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept override;
	void OnPaint_Impl(HWND hwnd) noexcept override;

public:

	class FlatButtonAppearance : public Object
	{
	public:

		// Gets or sets the color of the border around the button.
		Color BorderColor;

		// Gets or sets a value that specifies the size, in pixels, of the border around the button.
		int BorderSize;

		// Gets or sets the color of the client area of the button when the button is checked and the mouse pointer is outside the bounds of the control.
		Color CheckedBackColor;

		// Gets or sets the color of the client area of the button when the mouse is pressed within the bounds of the control.
		Color MouseDownBackColor;

		// Gets or sets the color of the client area of the button when the mouse pointer is within the bounds of the control.
		Color MouseOverBackColor;

		FlatButtonAppearance();
		virtual ~FlatButtonAppearance() = default;
	};

	FlatButtonAppearance FlatAppearance;
	FlatStyle FlatStyle;

	Button(Control* parent, const std::string& name, int width, int height, int x, int y);
	virtual ~Button();

	void Hide() override;
	void Initialize() override;
	void Show() override;
};

