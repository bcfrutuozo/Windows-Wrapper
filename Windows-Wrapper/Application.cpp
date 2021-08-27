#include "Application.h"

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

Window& Application::AddWindow(const std::string& name, int width, int height)
{
	auto w = new Window(name, width, height);
	Windows.Add(w);
	return *w;
}