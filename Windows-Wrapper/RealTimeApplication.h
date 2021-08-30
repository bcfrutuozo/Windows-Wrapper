#pragma once

#include "Application.h"
#include "Timer.h"

template<typename T>
class RealTimeApplication : public Application
{
private:

	static T App;
	static Timer Timer;

protected:

	RealTimeApplication() = default;
	RealTimeApplication(const RealTimeApplication&) = delete;			// Copy constructor
	RealTimeApplication(RealTimeApplication&&) = delete;				// Move constructor
	RealTimeApplication& operator=(const Application&) = delete;		// Copy assignment constructor
	RealTimeApplication& operator=(RealTimeApplication&&) = delete;		// Move assignment constructor
	~RealTimeApplication() = default;

public:

	virtual void HandleInput(float dt) = 0;
	virtual void Process(float dt) = 0;

	static const int Run()
	{
		App.Initialize();

		while (true)
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

			const auto dt = Timer.Peek() * SPEEDFACTOR;

			// AS NO GRAPHICS IS STILL IMPLEMENTED, SLEEP(1) IS NEEDED BECAUSE THE Window::ProcessMessages() IS USING PeekMessage() instead of GetMessage()
			// PeekMessage ALLOW THE PROGRAM FLOW TO CONTINUES WHILE GetMessage() WAITS FOR THE MESSAGE QUEUE
			// WHY DID I DO THIS?
			// I STILL DON'T KNOW, BUT IT'S BETTER TO START WITH THE HARD PART :D
			Sleep(1);
			App.HandleInput(dt);
			App.Process(dt);
		}
	}
};

template<typename T>
T RealTimeApplication<T>::App;

template<typename T>
Timer RealTimeApplication<T>::Timer;