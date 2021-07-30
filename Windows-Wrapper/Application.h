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

	static const std::optional<int> ProcessMessages()
	{
		MSG msg;

		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// Check for quit message because PeekMessage doesn't signal it via return val
			if (msg.message == WM_QUIT)
			{
				return static_cast<int>(msg.wParam);
			}

			// Translate and dispatch messages
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		return{};
	}

	int Start();
};
