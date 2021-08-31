#pragma once

#include "Control.h"

class Label final : public Control
{
private:

	BorderStyle m_BorderStyle;
	FlatStyle m_FlatStyle;

	// Singleton manages registration/cleanup of window class
	class LabelClass
	{
	private:

		static constexpr const char* m_ClassName = "Label Class";
		static LabelClass m_LabelClass;
		HINSTANCE m_Instance;

		LabelClass() noexcept;
		~LabelClass() noexcept;
		LabelClass(const LabelClass&) = delete;
		LabelClass& operator=(const LabelClass&) = delete;

	public:

		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	};

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

