#pragma once

#include "IActivable.h"
#include "IHidable.h"
#include "IWinControl.h"

class Button : public IWinControl<Button>, public IHidable, public IActivable
{
	friend class IWinControl;

private:

	bool IsMouseOver = false;
	bool IsClicking = false;

	// Member function responsible to handle the messages of each different type of control 
	LRESULT WINAPI HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept override;

public:

	// Singleton manages registration/cleanup of window class
	class BtnClass
	{
	private:

		static constexpr const char* m_ClassName = "Button Class";
		static BtnClass m_BtnClass;
		HINSTANCE m_Instance;

		BtnClass() noexcept;
		~BtnClass() noexcept;
		BtnClass(const BtnClass&) = delete;
		BtnClass& operator=(const BtnClass&) = delete;

	public:

		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	};

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

	enum FlatStyle
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
	FlatStyle FlatStyle = FlatStyle::Standard;

	Button(Control* parent, const std::string& name, int width, int height, int x, int y);
	virtual ~Button();

	void Disable() noexcept override;
	void Enable() noexcept override;
	void Hide() override;
	void Initialize() noexcept override;
	void Show() override;
};

