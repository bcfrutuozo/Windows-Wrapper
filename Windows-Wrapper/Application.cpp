#include "Application.h"

#include <stack>

Application::WindowCollection Application::Windows;

Application::Application()
{

}

Application::~Application()
{

}

void Application::AddWindow(Window* window)
{
	Windows.Add(window);
}

bool Application::ValidateWindows()
{
	// COMPLETE UNOPTIMIZED!
	// NEED TO IMPLEMENT A FASTER WAY TO HANDLE THIS
	std::stack<Window*> s;

	for (const auto w : Windows)
	{
		if (w->IsDisposing())
		{
			s.push(w);
		}
	}

	if (s.empty())
	{
		return true;
	}
	
	while (!s.empty())
	{
		auto& w = s.top();
		Windows.Remove(w);
		s.pop();
	}

	return true;
}

void Application::TryCloseApplication() noexcept
{
	if (Windows.GetCount() > 0)
	{
		return;
	}

	PostQuitMessage(0);
}

void Application::Exit() noexcept
{
	Windows.Clear();
	PostQuitMessage(0);
}