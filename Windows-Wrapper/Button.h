#pragma once

#include "Control.h"
#include "FlatButtonAppearance.h"

class Button final: public Control
{
	friend class WinAPI;

private:

	FlatButtonAppearance m_FlatAppearance;
	FlatStyle m_FlatStyle;

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

	Button(Control* parent, const std::string& name, int width, int height, int x, int y);
	virtual ~Button();

	void Initialize() override;
	FlatButtonAppearance GetFlatButtonAppearance() const noexcept;
	void SetFlatButtonAppearance(FlatButtonAppearance appearance) noexcept;
	FlatStyle GetFlatStyle() const noexcept;
	void SetFlatStyle(FlatStyle style) noexcept;
};

