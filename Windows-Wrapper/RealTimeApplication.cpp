#include "RealTimeApplication.h"
#include "Event.h"
#include "Timer.h"
#include "Menu.h"
#include "MenuStrip.h"
#include "MenuSeparator.h"
#include "MenuItem.h"
#include "MenuLeaf.h"

#include <sstream>
#include <iomanip>
#include <functional>

RealTimeApplication::RealTimeApplication()
{
	m_Window->GetMouse().DisableRaw();
	m_Window->GetKeyboard().EnableAutorepeat();
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

void RealTimeApplication::HandleInput(float dt)
{
	while (const auto& e = m_Window->GetKeyboard().ReadKey())
	{
		if (!e->IsPress())
		{
			switch (e->GetCode())
			{
			case VK_ESCAPE:
				if (m_Window->IsCursorEnabled())
				{
					m_Window->DisableCursor();
					m_Window->GetMouse().EnableRaw();
				}
				else
				{
					m_Window->EnableCursor();
					m_Window->GetMouse().DisableRaw();
				}
				break;
			}
		}
	}

	if (!m_Window->IsCursorEnabled())
	{
		if (m_Window->GetKeyboard().IsKeyPressed('Y'))
		{
			m_Window->AddButton("Teste", 200, 80, 50, 100);

			auto& mb = m_Window->GetMenuStrip();
			auto& item1 = mb.AddItem("Arquivo");
			auto& item2 = mb.AddItem("Arquivo2");
			auto& menu1 = mb.AddMenu("Submenu");
			menu1.AddItem("Submenu-Item1");
			menu1.AddCheckItem("Check-test01", true);
			menu1.AddCheckItem("Check-test02", false);
			menu1.AddSeparator();
			menu1.AddItemWithIcon("Submenu-Item2", "images\\1.bmp");
			menu1.AddSeparator();
			auto& menu2 = menu1.AddMenu("Submenu-Item3");
			menu2.AddItemWithIcon("Submenu2-Item1", "images\\2.bmp");
			menu2.AddSeparator();
			menu2.AddRadioItem("Radio1", true).Disable();
			menu2.AddRadioItem("Radio2", true);
			menu2.AddRadioItem("Radio3", false);
			menu2.AddRadioItem("Radio4", true);
			menu2.AddSeparator();
			menu2.AddRadioItem("Radio1", false);
			menu2.AddRadioItem("Radio2", true);
			menu1.AddItem("Submenu-Item4");
			menu1.AddItem("Submenu-Item5");
			m_Window->UpdateMenuStrip();
		}

		if (m_Window->GetKeyboard().IsKeyPressed('R'))
		{
			m_Window->ClearMenuStrip();
		}

		if (m_Window->GetKeyboard().IsKeyPressed('O'))
		{
			m_Window->GetMenuStrip().Show();
		}

		if (m_Window->GetKeyboard().IsKeyPressed('P'))
		{
			m_Window->GetMenuStrip().Hide();
		}

		if (m_Window->GetKeyboard().IsKeyPressed('Q'))
		{
			/*Color x = m_Window->();
			x.SetR(x.GetR() - 1);

			if (x.GetR() >= 0) m_Window->SetForeColor(x);*/
			//cameras->Translate({ 0.0f, 0.0f, dt });
		}
		if (m_Window->GetKeyboard().IsKeyPressed('A'))
		{
			/*Color x = m_Window->GetForeColor();
			x.SetG(x.GetG() - 1);

			if (x.GetG() >= 0) m_Window->SetForeColor(x);*/
			//cameras->Translate({ -dt, 0.0f, 0.0f });
		}
		if (m_Window->GetKeyboard().IsKeyPressed('Z'))
		{
			/*Color x = m_Window->GetForeColor();
			x.SetB(x.GetB() - 1);

			if (x.GetB() >= 0) m_Window->SetForeColor(x);*/
			//cameras->Translate({ 0.0f, 0.0f, -dt });
		}
		if (m_Window->GetKeyboard().IsKeyPressed('W'))
		{
			/*Color x = m_Window->GetForeColor();
			x.SetR(x.GetR() + 1);

			if (x.GetR() < 256) m_Window->SetForeColor(x);*/

			//cameras->Translate({ dt, 0.0f, 0.0f });
		}
		if (m_Window->GetKeyboard().IsKeyPressed('S'))
		{
			/*Color x = m_Window->GetForeColor();
			x.SetG(x.GetG() + 1);

			if (x.GetG() < 256) m_Window->SetForeColor(x);*/

			//cameras->Translate({ 0.0f, dt, 0.0f });
		}
		if (m_Window->GetKeyboard().IsKeyPressed('X'))
		{
			/*Color x = m_Window->GetForeColor();
			x.SetB(x.GetB() + 1);

			if (x.GetB() < 256) m_Window->SetForeColor(x);*/

			//cameras->Translate({ 0.0f, -dt, 0.0f });
		}
	}


	while (const auto& delta = m_Window->GetMouse().ReadRawDelta())
	{
		if (!m_Window->IsCursorEnabled())
		{
			//cameras->Rotate(static_cast<float>(delta->x), static_cast<float>(delta->y));
		}
	}
}

void RealTimeApplication::Run(float dt)
{
	// Time elapse on window title
	//const float t = m_Timer.Peek();
	std::ostringstream oss;
	oss << "Timer elapsed: " << std::setprecision(1) << std::fixed << dt << "s";
	m_Window->SetText(oss.str().c_str());

	// renderGraph.Execute(window.Gfx());

	// Present the frame
	//window.Gfx().EndFrame();
	//renderGraph.Reset();
}