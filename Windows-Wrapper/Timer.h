#pragma once

#include "CommonObject.h"

class Timer : public Object
{
private:

	std::chrono::steady_clock::time_point m_Last;

public:

	Timer() noexcept;
	const float Mark() noexcept;
	const float Peek() const noexcept;
};