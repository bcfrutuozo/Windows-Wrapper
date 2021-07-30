#pragma once

#include "Control.h"
#include "IActivable.h"
#include "IHidable.h"
#include "WinControl.h"

class Button : public Control, public WinControl<Button>, public IHidable, public IActivable
{
	friend class WinControl;

private:

	LRESULT WINAPI HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept override;

	// Singleton manages registration/cleanup of button class
	class WinButton
	{
	private:

		static constexpr const char* m_ClassName = "Button Class";
		static WinButton m_WinButton;
		HINSTANCE m_Instance;

		WinButton() noexcept;
		~WinButton() noexcept;
		WinButton(const WinButton&) = delete;
		WinButton& operator=(const WinButton&) = delete;

	public:

		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	};

public:

	Button(Control* parent, const std::string& name, int width, int height, int x, int y);
	virtual ~Button();

	void Disable() noexcept override;
	void Enable() noexcept override;
	void Hide() override;
	void Initialize() noexcept override;
	void Show() override;
};

