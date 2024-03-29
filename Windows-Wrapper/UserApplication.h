#pragma once

#include "Application.h"

template<typename T>
class UserApplication : public Application
{
private:

	static T App;

protected:

	UserApplication() = default;
	UserApplication(const UserApplication&) = delete;				// Copy constructor
	UserApplication(UserApplication&&) = delete;					// Move constructor
	UserApplication& operator=(const UserApplication&) = delete;	// Copy assignment constructor
	UserApplication& operator=(UserApplication&&) = delete;			// Move assignment constructor
	virtual ~UserApplication() = default;

public:

	static const int Run()
	{
		Start();

		App.Initialize();

		MSG msg;

		while (GetMessage(&msg, nullptr, 0, 0) != 0)
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

		Stop();

		return 0;
	}
};

template<typename T>
T UserApplication<T>::App;