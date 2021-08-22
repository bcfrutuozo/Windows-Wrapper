#pragma once

#include "WinControl.h"
#include "Enums.h"
#include "Timer.h"

#include <thread>

class ProgressBar : public WinControl
{
private:

	volatile int m_Value;
	ProgressBarAnimation m_Animation;
	volatile bool m_IsRunning;
	volatile int Minimum;
	volatile int Maximum;
	std::thread m_UpdateThread;
	volatile int m_Speed;

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

	int OnEraseBackground_Impl(HWND hwnd, HDC hdc) noexcept;
	void OnPaint_Impl(HWND hwnd) noexcept override;
	void OnPaintMarquee_Thread(HWND hwnd) noexcept;

public:

	int Step;

	ProgressBar(Control* parent, int width, int height, int x, int y);
	ProgressBar(Control* parent, const std::string& name, int width, int height, int x, int y);
	virtual ~ProgressBar();

	void Initialize() override;
	int GetValue() const noexcept;
	void SetValue(int value) noexcept;
	void IncrementStep() noexcept;
	void DecrementStep() noexcept;
	void SetMinimum(int value) noexcept;
	void SetMaximum(int value) noexcept;
	void SetSpeed(int value) noexcept;
	ProgressBarAnimation GetAnimationType() const noexcept;
	void SetAnimation(ProgressBarAnimation animation) noexcept;
	void Start() noexcept;
	void Stop() noexcept;
};

