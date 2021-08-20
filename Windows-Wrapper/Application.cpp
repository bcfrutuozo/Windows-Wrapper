#include "Application.h"

Application::Application()
	:
	m_Window(std::make_unique<Window>("Application", 1024, 1000))
{

}

Application::~Application()
{

}

Window* const Application::GetWindow() const noexcept
{
	return m_Window.get();
}
