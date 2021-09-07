#pragma once

#include "Common.h"
#include "Collection.h"
#include "Window.h"

class Application
{
	friend class Window;
	friend class WinAPI;

private:

	static void AddWindow(Window* window);
	static bool RemoveWindow(Window* window);

public:

	class WindowCollection final : public Collection<Window>
	{

	};

protected:

	static WindowCollection Windows;

	Application() = default;
	Application(const Application&) = delete;				// Copy constructor
	Application(Application&&) = delete;					// Move constructor
	Application& operator=(const Application&) = delete;	// Copy assignment constructor
	Application& operator=(Application&&) = delete;			// Move assignment constructor
	~Application() = default;

public:

	virtual void Initialize() = 0;
	static void TryCloseApplication() noexcept;
	static void Exit() noexcept;
};