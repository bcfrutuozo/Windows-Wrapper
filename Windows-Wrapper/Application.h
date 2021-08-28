#pragma once

#include "Window.h"

class Application
{
public:

	class WindowCollection final : public Collection<Window>
	{

	};

protected:

	static WindowCollection Windows;

	Application();
	~Application();
	Application(const Application&) = delete;				// Copy constructor
	Application(Application&&) = delete;					// Move constructor
	Application& operator=(const Application&) = delete;	// Copy assignment constructor
	Application& operator=(Application&&) = delete;			// Move assignment constructor

public:

	void AddWindow(Window* window);
	static bool ValidateWindows();
	virtual void Initialize() = 0;
	static void TryCloseApplication() noexcept;
	static void Exit() noexcept;
};