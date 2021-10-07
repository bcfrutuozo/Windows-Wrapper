#include "Application.h"
#include "Window.h"
#include "Direct2D.h"

bool Application::m_IsRunning = false;
bool Application::m_HasGraphicsChanged = false;
GraphicsType Application::m_SetGraphicsType = GraphicsType::GDI;

std::list<Window*>* Application::Windows = new std::list<Window*>();

void Application::AddWindow(Window* window)
{
	Windows->push_back(window);
}

bool Application::RemoveWindow(Window* window)
{
	if (std::contains(*Windows, window) && window->IsDisposed())
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

void Application::Stop() noexcept
{
	m_IsRunning = false;
}

void Application::Start() noexcept
{
	m_IsRunning = true;
	m_HasGraphicsChanged = false;
}

void Application::SetGraphicsType(GraphicsType graphicsType) noexcept
{
	m_SetGraphicsType = graphicsType; 
	m_HasGraphicsChanged = true;
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

	SafeDelete(Windows);

	if (m_SetGraphicsType == GraphicsType::D2D) Direct2D::DiscardDeviceResources();

	PostQuitMessage(0);
}