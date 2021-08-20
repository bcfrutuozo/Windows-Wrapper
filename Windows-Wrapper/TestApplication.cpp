#include "TestApplication.h"
#include "KeyEventArgs.h"

#include "Event.h"
#include "Timer.h"
#include "Menu.h"
#include "MenuStrip.h"
#include "MenuSeparator.h"
#include "MenuItem.h"
#include "MenuLeaf.h"

void TestKeyPress(Control* c, KeyEventArgs* e)
{
	ProgressBar* teste = dynamic_cast<ProgressBar*>(c);

	if (c == nullptr)
	{
		return;
	}

	if (e->KeyData == Keys::W)
	{
		if (teste->GetAnimationType() == ProgressBarAnimation::Marquee)
		{
			teste->SetAnimation(ProgressBarAnimation::Blocks);
		}
		else
		{
			teste->SetAnimation(ProgressBarAnimation::Marquee);
		}
	}

	if (e->KeyData == Keys::A)
	{
		teste->Start();
	}
	if (e->KeyData == Keys::S)
	{
		teste->Stop();
	}
	if (e->KeyData == Keys::U)
	{
		teste->SetValue(teste->GetValue() + 1);
	}
	if (e->KeyData == Keys::I)
	{
		teste->SetValue(teste->GetValue() - 1);

	}
	if (e->KeyData == Keys::O)
	{
		teste->IncrementStep();
	}
	if (e->KeyData == Keys::P)
	{
		teste->DecrementStep();
	}
}

void TestApplication::Initialize()
{
	auto window = GetWindow();

	window->GetMouse().EnableRaw();
	window->GetKeyboard().DisableAutorepeat();

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

	button1 = &window->AddButton("Abc", 200, 80, 50, 100);
	button1->OnClickSet([](Control* c, EventArgs* e)
		{
			OutputDebugString("Teste Click Button1");
		}
	);
	button1->OnMouseEnterSet([](Control* c, EventArgs* e)
		{
			OutputDebugString("Teste Mouseenter Button1");
		}
	);
	button1->OnMouseLeaveSet([](Control* c, EventArgs* e)
		{
			OutputDebugString("Teste Mouseleave Button1");
		}
	);

	button2 = &window->AddButton("XYZ", 100, 40, 200, 200);
	button2->OnClickSet([](Control* c, EventArgs* e)
		{
			OutputDebugString("Teste Click Button2");
		}
	);
	button2->Font.SetStyle(FontStyle::Strikeout);

	txtBox1 = &window->AddTextBox("TextBox", 400, 20, 400);
	txtBox1->BorderStyle = BorderStyle::None;

	txtBox2 = &window->AddTextBox("TextBox2", 400, 20, 460);
	txtBox2->BorderStyle = BorderStyle::FixedSingle;

	txtBox3 = &window->AddTextBox("TextBox3", 400, 20, 560);
	txtBox3->BorderStyle = BorderStyle::Fixed3D;

	progressBar = &window->AddProgressBar("abc", 300, 50, 400, 20);
	progressBar->OnKeyDownSet(&TestKeyPress);
}