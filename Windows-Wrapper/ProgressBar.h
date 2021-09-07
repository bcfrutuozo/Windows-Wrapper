#pragma once

#include "Control.h"
#include "Enums.h"

#include <thread>

class ProgressBar final: public Control
{
private:

	volatile int m_Value;
	ProgressBarAnimation m_Animation;
	volatile bool m_IsRunning;
	volatile int Minimum;
	volatile int Maximum;
	std::thread m_UpdateThread;
	volatile int m_Speed;

	int OnEraseBackground_Impl(HWND hwnd, HDC hdc) noexcept override;
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

