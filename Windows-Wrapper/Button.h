#pragma once

#include "IActivable.h"
#include "IHidable.h"
#include "WinControl.h"

class Button : public WinControl, public IHidable, public IActivable
{
	friend class WinControl;

private:

	bool m_TabClicking;

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

	void OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept override;
	void OnKeyUp_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept override;
	void OnPaint_Impl(HWND hwnd) noexcept override;

public:

	class FlatButtonAppearance : public Base
	{
	public:

		// Gets or sets the color of the border around the button.
		Color BorderColor = Color::Border();
			
		//Gets or sets a value that specifies the size, in pixels, of the border around the button.
		int BorderSize = 1;
			
		//Gets or sets the color of the client area of the button when the button is checked and the mouse pointer is outside the bounds of the control.
		Color CheckedBackColor;

		//Gets or sets the color of the client area of the button when the mouse is pressed within the bounds of the control.
		Color MouseDownBackColor = Color(204, 228, 247, 255);

		//Gets or sets the color of the client area of the button when the mouse pointer is within the bounds of the control.
		Color MouseOverBackColor = Color(229, 241, 251, 255);

	};

	enum class FlatStyle
	{
		//The control appears flat.
		Flat = 0,

		//A control appears flat until the mouse pointer moves over it, at which point it appears three - dimensional.
		Popup = 1,

		//The control appears three - dimensional.
		Standard = 2,

		//The appearance of the control is determined by the user's operating system.
		System = 3
	};

	FlatButtonAppearance FlatAppearance;
	FlatStyle FlatStyle;

	Button(Control* parent, const std::string& name, int width, int height, int x, int y);
	virtual ~Button();

	void Disable() noexcept override;
	void Enable() noexcept override;
	void Hide() override;
	void Initialize() noexcept override;
	void Show() override;
};

