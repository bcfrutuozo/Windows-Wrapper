#pragma once

#include "Window.h"
#include "Timer.h"

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

constexpr auto SPEEDFACTOR = 1.0f;

class Application
{

private:
	void Run(float dt);
	void HandleInput(float dt);

	Timer m_Timer;
	Window m_Window;

public:
	Application();
	~Application();

	int Start();
};
