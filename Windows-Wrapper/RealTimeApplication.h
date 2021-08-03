#pragma once

#include "Application.h"
#include "Timer.h"

constexpr auto SPEEDFACTOR = 1.0f;

class RealTimeApplication : public Application
{

private:
	void Run(float dt);
	void HandleInput(float dt);

	Timer m_Timer;

public:

	RealTimeApplication();
	~RealTimeApplication();
	RealTimeApplication(const RealTimeApplication&) = delete;			// Copy constructor
	RealTimeApplication(RealTimeApplication&&) = delete;				// Move constructor
	RealTimeApplication& operator=(const Application&) = delete;		// Copy assignment constructor
	RealTimeApplication& operator=(RealTimeApplication&&) = delete;		// Move assignment constructor

	const int Start() override;
};
