#include "Timer.h"
#include <chrono>

Timer::Timer() noexcept
	:
	m_Last(std::chrono::steady_clock::now())
{

}

const float Timer::Mark() noexcept
{
	const auto old = m_Last;
	m_Last = std::chrono::steady_clock::now();
	const std::chrono::duration<float> frameTime = m_Last - old;
	return frameTime.count();
}

const float Timer::Peek() const noexcept
{
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - m_Last).count();
}