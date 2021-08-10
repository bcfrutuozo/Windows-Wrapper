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

}

void TesteBotaoLeave(Control* const c, EventArgs* const e)
{

}

void TesteBotaoClick(Control* const c, EventArgs* const e)
{
	OutputDebugString("FUNFOUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU");
}

void TestKeyPress(Control* const c, KeyEventArgs* e)
{
	if (e->KeyData == Keys::Y)
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
		menu1.AddItem("Submenu-Item5").OnMouseEnterSet(&TesteBotaoEnter);
		window->UpdateMenuStrip();

		Button& b = window->AddButton("Abc", 200, 80, 50, 100);
		b.OnClickSet(&TesteBotaoClick);
		b.OnMouseEnterSet(&TesteBotaoEnter);
		b.OnMouseLeaveSet(&TesteBotaoLeave);

		Button& b2 = window->AddButton("XYZ", 100, 40, 200, 200);
		b2.OnClickSet(&TesteBotaoClick);
		b2.Font.SetStyle(FontStyle::Strikeout);

		TextBox& t = window->AddTextBox("TextBox", 400, 30, 20, 400);
		t.Font.SetSize(10);
		t.BorderStyle = BorderStyle::None;

		TextBox& t2 = window->AddTextBox("TextBox2", 400, 30, 20, 460);
		t2.BorderStyle = BorderStyle::FixedSingle;

		TextBox& t3 = window->AddTextBox("TextBox3", 400, 30, 20, 560);
		t3.BorderStyle = BorderStyle::Fixed3D;
	}
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