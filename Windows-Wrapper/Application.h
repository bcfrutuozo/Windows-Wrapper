#pragma once

#include "Window.h"

class Application
{
private:

	std::unique_ptr<Window> m_Window;
	
public:

	Application();
	~Application();
	Application(const Application&) = delete;				// Copy constructor
	Application(Application&&) = delete;					// Move constructor
	Application& operator=(const Application&) = delete;	// Copy assignment constructor
	Application& operator=(Application&&) = delete;			// Move assignment constructor

	virtual void Initialize() = 0;
	virtual const int Start() = 0;
	Window* const GetWindow() const noexcept;
};