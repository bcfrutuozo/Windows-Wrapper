#include "Application.h"

Application::Application()
	:
	m_Window(std::make_unique<Window>("Application", 1920, 1080))
{

}

Application::~Application()
{

}