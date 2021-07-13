#include "Application.h"
#include "Event.h"
#include "Timer.h"

#include <sstream>
#include <iomanip>
#include <functional>

Application::Application()
	:
	m_Window(1920, 1080, "Application")
{
	m_Window.EnableCursor();
	m_Window.GetMouse().DisableRaw();
}

Application::~Application()
{ }

int Application::Start()
{
	while (true)
	{
		// Process all messages pending but doesn't block new messages
		if (const auto& ecode = Window::ProcessMessages())
		{
			// if return option has value exits the application
			return *ecode;
		}


		const auto dt = m_Timer.Peek() * SPEEDFACTOR;

		// AS NO GRAPHICS IS STILL IMPLEMENTED, SLEEP(1) IS NEEDED BECAUSE THE Window::ProcessMessages() IS USING PeekMessage() instead of GetMessage()
		// PeekMessage ALLOW THE PROGRAM FLOW TO CONTINUES WHILE GetMessage() WAITS FOR THE MESSAGE QUEUE
		// WHY DID I DO THIS?
		// I STILL DON'T KNOW, BUT IT'S BETTER TO START WITH THE HARD PART :D
		Sleep(1);
		HandleInput(dt);
		Run(dt);
	}
}

void OpenFileX()
{

}

void Application::HandleInput(float dt)
{
	while (const auto& e = m_Window.GetKeyboard().ReadKey())
	{
		if (!e->IsPress())
		{
			switch (e->GetCode())
			{
			case VK_ESCAPE:
				if (m_Window.IsCursorEnabled())
				{
					m_Window.DisableCursor();
					m_Window.GetMouse().EnableRaw();
				}
				else
				{
					m_Window.EnableCursor();
					m_Window.GetMouse().DisableRaw();
				}
				break;
			}
		}
	}

	if (!m_Window.IsCursorEnabled())
	{
		if (m_Window.GetKeyboard().IsKeyPressed('Y'))
		{
			auto m = m_Window.CreateMenu("Teste");
			m.AddEntry(MenuItem::MENU_ITEMTYPE_STRING, "Arquivo", new Event<void()>("Teste", &OpenFileX));
		}

		if (m_Window.GetKeyboard().IsKeyPressed('Q'))
		{
			Color x = m_Window.GetForeColor();
			x.SetR(x.GetR() - 1);
			
			if (x.GetR() >= 0) m_Window.SetForeColor(x);
			//cameras->Translate({ 0.0f, 0.0f, dt });
		}
		if (m_Window.GetKeyboard().IsKeyPressed('A'))
		{
			Color x = m_Window.GetForeColor();
			x.SetG(x.GetG() - 1);

			if (x.GetG() >= 0) m_Window.SetForeColor(x);
			//cameras->Translate({ -dt, 0.0f, 0.0f });
		}
		if (m_Window.GetKeyboard().IsKeyPressed('Z'))
		{
			Color x = m_Window.GetForeColor();
			x.SetB(x.GetB() - 1);

			if(x.GetB() >= 0) m_Window.SetForeColor(x);
			//cameras->Translate({ 0.0f, 0.0f, -dt });
		}
		if (m_Window.GetKeyboard().IsKeyPressed('W'))
		{
			Color x = m_Window.GetForeColor();
			x.SetR(x.GetR() + 1);

			if (x.GetR() < 256) m_Window.SetForeColor(x);

			//cameras->Translate({ dt, 0.0f, 0.0f });
		}
		if (m_Window.GetKeyboard().IsKeyPressed('S'))
		{
			Color x = m_Window.GetForeColor();
			x.SetG(x.GetG() + 1);

			if (x.GetG() < 256) m_Window.SetForeColor(x);

			//cameras->Translate({ 0.0f, dt, 0.0f });
		}
		if (m_Window.GetKeyboard().IsKeyPressed('X'))
		{
			Color x = m_Window.GetForeColor();
			x.SetB(x.GetB() + 1);

			if (x.GetB() < 256) m_Window.SetForeColor(x);

			//cameras->Translate({ 0.0f, -dt, 0.0f });
		}
	}


	while (const auto& delta = m_Window.GetMouse().ReadRawDelta())
	{
		if (!m_Window.IsCursorEnabled())
		{
			//cameras->Rotate(static_cast<float>(delta->x), static_cast<float>(delta->y));
		}
	}
}

void Application::Run(float dt)
{
	// Time elapse on window title
	//const float t = m_Timer.Peek();
	std::ostringstream oss;
	oss << "Timer elapsed: " << std::setprecision(1) << std::fixed << dt << "s";
	m_Window.SetTitle(oss.str().c_str());

	// renderGraph.Execute(window.Gfx());

	// Present the frame
	//window.Gfx().EndFrame();
	//renderGraph.Reset();
}