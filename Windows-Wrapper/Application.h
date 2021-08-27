#pragma once

#include "Window.h"

class Application
{
public:

	class WindowCollection final : public Collection<Window>
	{

	};

private:

	WindowCollection Windows;

public:

	Application();
	~Application();
	Application(const Application&) = delete;				// Copy constructor
	Application(Application&&) = delete;					// Move constructor
	Application& operator=(const Application&) = delete;	// Copy assignment constructor
	Application& operator=(Application&&) = delete;			// Move assignment constructor

	void AddWindow(Window* window);
	Window& AddWindow(const std::string& name, int width, int height);
	virtual void Initialize() = 0;
	virtual const int Run() = 0;
};