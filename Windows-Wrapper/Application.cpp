#include "Application.h"
#include "WinAPI.h"
#include "Window.h"

Application::WindowCollection Application::Windows;

void Application::AddWindow(Window* window)
{
	Windows.Add(window);
}

bool Application::RemoveWindow(Window* window)
{
	if (Windows.Contains(window) && window->IsDisposing())
	{
		return Windows.Remove(window);
	}

	return false;
}

void Application::TryCloseApplication() noexcept
{
	if (Windows.IsEmpty())
	{
		PostQuitMessage(0);
	}	
}

void Application::Exit() noexcept
{
	Windows.Clear();
	PostQuitMessage(0);
}