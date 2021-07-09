#pragma once

#include <chrono>

class Timer
{
private:

	std::chrono::steady_clock::time_point m_Last;

public:

	Timer() noexcept;
	const float Mark() noexcept;
	const float Peek() const noexcept;
};