#include "UserApplication.h"
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

void TesteBotaoEnter(Control* const c, EventArgs* const e)
{
	c->SetBackgroundColor(Color::Blue());
	c->SetForeColor(Color::Red());
}

void TesteBotaoLeave(Control* const c, EventArgs* const e)
{
	c->SetBackgroundColor(Color::Default());
	c->SetForeColor(Color::Black());
}

void TesteBotaoClick(Control* const c, EventArgs* const e)
{
	OutputDebugString("FUNFOUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU");
}

void TestKeyPress(Control* const c, KeyEventArgs* e)
{
	auto window = dynamic_cast<Window*>(c);

	auto& mb = window->GetMenuStrip();
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
	window->UpdateMenuStrip();
	window->SetBackgroundColor(Color(128, 128, 128));

	Button& b = window->AddButton("Abc", 200, 80, 50, 100);
	b.OnClickSet(&TesteBotaoClick);
	b.OnMouseEnterSet(&TesteBotaoEnter);
	b.OnMouseLeaveSet(&TesteBotaoLeave);
}


UserApplication::UserApplication()
{
	m_Window->GetMouse().EnableRaw();
	m_Window->GetKeyboard().DisableAutorepeat();
	m_Window->OnKeyUpSet(&TestKeyPress);
}

UserApplication::~UserApplication()
{ }

const int UserApplication::Start()
{
	MSG msg;

	while (GetMessage(&msg, nullptr, 0, 0) != 0)
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
}