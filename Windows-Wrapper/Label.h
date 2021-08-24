#pragma once

#include "Control.h"

class Label final : public Control
{
private:

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

protected:

public:

	Label(Control* parent, const std::string& text, int x, int y);
	virtual ~Label() = default;

	void Initialize() override;
};

