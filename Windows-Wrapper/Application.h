#pragma once

#include "Window.h"
#include "Timer.h"

#include <memory>

constexpr auto SPEEDFACTOR = 1.0f;

class Application
{

private:
	void Run(float dt);
	void HandleInput(float dt);

	Timer m_Timer;
	std::unique_ptr<Window> m_Window;

public:
	Application();
	~Application();

	int Start();
};
