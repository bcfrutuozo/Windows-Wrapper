#pragma once

#include "Control.h"
#include "IActivable.h"
#include "IHidable.h"
#include "IWinControl.h"

class Button : public IWinControl<Button>, public IHidable, public IActivable
{
	friend class IWinControl;

private:

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

	LRESULT WINAPI HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept override;

public:

	Button(Control* parent, const std::string& name, int width, int height, int x, int y);
	virtual ~Button();

	void Disable() noexcept override;
	void Enable() noexcept override;
	void Hide() override;
	void Initialize() noexcept override;
	void Show() override;
};

