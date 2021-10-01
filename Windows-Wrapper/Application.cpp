#include "Application.h"
#include "Window.h"

std::list<Window*>* Application::Windows = new std::list<Window*>();

void Application::AddWindow(Window* window)
{
	Windows->push_back(window);
}

bool Application::RemoveWindow(Window* window)
{
	if (std::contains(*Windows, window) && window->IsDisposing())
	{
		if (Windows->remove(window))
		{
			delete window;
			window = nullptr;
			return true;
		}
	}

	return false;
}

bool Application::CanCloseApplication() noexcept
{
	return (Windows->size() == 0);
}

void Application::Exit() noexcept
{
	for (auto window : *Windows)
	{
		// Call dispose here because it wasn't called on the close window event
		window->Dispose();
		RemoveWindow(window);
		delete window;
		window = nullptr;
	}

	delete Windows;
	Windows = nullptr;
	PostQuitMessage(0);
}