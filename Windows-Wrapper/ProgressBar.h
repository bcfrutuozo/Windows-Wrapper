#pragma once

#include "WinControl.h"

class ProgressBar : public WinControl
{
private:

	unsigned int m_Value;

	// Singleton manages registration/cleanup of window class
	class ProgressBarClass
	{
	private:

		static constexpr const char* m_ClassName = "Progress Bar Class";
		static ProgressBarClass m_ProgressBarClass;
		HINSTANCE m_Instance;

		ProgressBarClass() noexcept;
		~ProgressBarClass() noexcept;
		ProgressBarClass(const ProgressBarClass&) = delete;
		ProgressBarClass& operator=(const ProgressBarClass&) = delete;

	public:

		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	};

	void OnPaint_Impl(HWND hwnd) noexcept override;

public:

	unsigned int Step;
	unsigned int Minimum;
	unsigned int Maximum;

	ProgressBar(Control* parent, int width, int height, int x, int y);
	ProgressBar(Control* parent, const std::string& name, int width, int height, int x, int y);
	virtual ~ProgressBar();

	void Initialize() noexcept override;
	unsigned int GetValue() const noexcept;
	void SetValue(int value) noexcept;
	void IncrementStep() noexcept;
	void DecrementStep() noexcept;
};

