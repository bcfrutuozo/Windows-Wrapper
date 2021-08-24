#include "RealTimeApplication.h"
#include "Event.h"
#include "Timer.h"
#include "Menu.h"
#include "MenuBar.h"
#include "MenuSeparator.h"
#include "MenuItem.h"
#include "MenuLeaf.h"

#include <sstream>
#include <iomanip>
#include <functional>

RealTimeApplication::RealTimeApplication()
{
	auto window = GetWindow();
	window->GetMouse().DisableRaw();
	window->GetKeyboard().EnableAutorepeat();
}

RealTimeApplication::~RealTimeApplication()
{ }

const int RealTimeApplication::Start()
{
	while (true)
	{
		MSG msg;

		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// Check for quit message because PeekMessage doesn't signal it via return val
			if (msg.message == WM_QUIT)
			{
				return static_cast<int>(msg.wParam);
			}

			// Translate and dispatch messages
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		const auto dt = Timer.Peek() * SPEEDFACTOR;

		// AS NO GRAPHICS IS STILL IMPLEMENTED, SLEEP(1) IS NEEDED BECAUSE THE Window::ProcessMessages() IS USING PeekMessage() instead of GetMessage()
		// PeekMessage ALLOW THE PROGRAM FLOW TO CONTINUES WHILE GetMessage() WAITS FOR THE MESSAGE QUEUE
		// WHY DID I DO THIS?
		// I STILL DON'T KNOW, BUT IT'S BETTER TO START WITH THE HARD PART :D
		Sleep(1);
		HandleInput(dt);
		Run(dt);
	}
}

void RealTimeApplication::HandleInput(float dt)
{
	auto window = GetWindow();

	while (const auto& e = window->GetKeyboard().ReadKey())
	{
		if (!e->IsPress())
		{
			switch (e->GetCode())
			{
			case VK_ESCAPE:
				if (window->IsCursorEnabled())
				{
					window->DisableCursor();
					window->GetMouse().EnableRaw();
				}
				else
				{
					window->EnableCursor();
					window->GetMouse().DisableRaw();
				}
				break;
			}
		}
	}

	if (!window->IsCursorEnabled())
	{
		if (window->GetKeyboard().IsKeyPressed('Y'))
		{
			
		}

		if (window->GetKeyboard().IsKeyPressed('R'))
		{

		}

		if (window->GetKeyboard().IsKeyPressed('O'))
		{

		}

		if (window->GetKeyboard().IsKeyPressed('P'))
		{
		}

		if (window->GetKeyboard().IsKeyPressed('Q'))
		{
			/*Color x = m_Window->();
			x.SetR(x.GetR() - 1);

			if (x.GetR() >= 0) m_Window->SetForeColor(x);*/
			//cameras->Translate({ 0.0f, 0.0f, dt });
		}
		if (window->GetKeyboard().IsKeyPressed('A'))
		{
			/*Color x = m_Window->GetForeColor();
			x.SetG(x.GetG() - 1);

			if (x.GetG() >= 0) m_Window->SetForeColor(x);*/
			//cameras->Translate({ -dt, 0.0f, 0.0f });
		}
		if (window->GetKeyboard().IsKeyPressed('Z'))
		{
			/*Color x = m_Window->GetForeColor();
			x.SetB(x.GetB() - 1);

			if (x.GetB() >= 0) m_Window->SetForeColor(x);*/
			//cameras->Translate({ 0.0f, 0.0f, -dt });
		}
		if (window->GetKeyboard().IsKeyPressed('W'))
		{
			/*Color x = m_Window->GetForeColor();
			x.SetR(x.GetR() + 1);

			if (x.GetR() < 256) m_Window->SetForeColor(x);*/

			//cameras->Translate({ dt, 0.0f, 0.0f });
		}
		if (window->GetKeyboard().IsKeyPressed('S'))
		{
			/*Color x = m_Window->GetForeColor();
			x.SetG(x.GetG() + 1);

			if (x.GetG() < 256) m_Window->SetForeColor(x);*/

			//cameras->Translate({ 0.0f, dt, 0.0f });
		}
		if (window->GetKeyboard().IsKeyPressed('X'))
		{
			/*Color x = m_Window->GetForeColor();
			x.SetB(x.GetB() + 1);

			if (x.GetB() < 256) m_Window->SetForeColor(x);*/

			//cameras->Translate({ 0.0f, -dt, 0.0f });
		}
	}


	while (const auto& delta = window->GetMouse().ReadRawDelta())
	{
		if (!window->IsCursorEnabled())
		{
			//cameras->Rotate(static_cast<float>(delta->x), static_cast<float>(delta->y));
		}
	}
}

void RealTimeApplication::Run(float dt)
{
	auto window = GetWindow();

	// Time elapse on window title
	//const float t = m_Timer.Peek();
	std::ostringstream oss;
	oss << "Timer elapsed: " << std::setprecision(1) << std::fixed << dt << "s";
	window->SetText(oss.str().c_str());

	// renderGraph.Execute(window.Gfx());

	// Present the frame
	//window.Gfx().EndFrame();
	//renderGraph.Reset();
}