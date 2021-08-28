#pragma once

#include "Application.h"

class TestApplication;

template<typename T>
class UserApplication : public Application
{
private:

	static T App;

protected:

	UserApplication()
	{ }

	virtual ~UserApplication()
	{ }

	UserApplication(const UserApplication&) = delete;				// Copy constructor
	UserApplication(UserApplication&&) = delete;					// Move constructor
	UserApplication& operator=(const UserApplication&) = delete;	// Copy assignment constructor
	UserApplication& operator=(UserApplication&&) = delete;			// Move assignment constructor

public:

	static const int Run()
	{
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

		return 0;
	}

	static double x;
};

template<typename T>
T UserApplication<T>::App;