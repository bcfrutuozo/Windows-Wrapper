#include "TestUserApplication.h"
#include "KeyEventArgs.h"
#include "ToolStrip.h"
#include "ListBox.h"
#include "Color.h"
#include "Int32.h"

#include "Event.h"
#include "Timer.h"

void TestClick(Object* sender, EventArgs* e)
{
	Button* b = (Button*)sender;
	b->Dispose();
}

void TestUserApplication::Initialize()
{
	Int32 a = 10;
	Int32 b;
	a += 5;
	auto z = (a + 2);
	a++;
	Window* window = new Window("Teste", 1000, 1000);
	window->GetMouse().EnableRaw();
	window->GetKeyboard().DisableAutorepeat();

	auto lb = window->AddListBox(260, 240, 450, 50);

	auto list = new ListItemCollection(lb);
	for (size_t i = 0; i < 1000; ++i)
	{
		std::ostringstream oss;
		oss << "Item: " << i;
		list->Add(new ListItem(i, oss.str()));
	}
	lb->SetDataSource(list);
	auto f = lb->GetFont();
	f.SetStyle(FontStyle::Italic);
	f.SetSize(14);
	lb->SetFont(f);
	lb->SetBorderStyle(BorderStyle::Fixed3D);
	//lb->EnableMultiColumn();
	lb->SetColumnWidth(180);

	auto cb = window->AddComboBox("Start Text", 260, 450, 450);
	// Double list to check for memory leak during DataSource rebinding
	auto list2 = new ListItemCollection(lb);
	for (size_t i = 0; i < 5; ++i)
	{
		std::ostringstream oss;
		oss << "Item: " << i;
		list2->Add(new ListItem(i, oss.str()));
	}
	cb->SetDataSource(list2);

	//auto t1 = window->AddComboBox("ComboBox", 300, 120, 120);
	//window2->AddButton("Teste", 250, 250, 125, 125);

	//ToolStrip& p = window->AddToolStrip();
	//ToolStrip& j = window->AddToolStrip();
	//p.SetBackgroundColor(Color::Black());
	//j.SetBackgroundColor(Color::Blue());
	//auto l = window->AddLabel("abcdef", 300, 100);
	//auto f = l->GetFont();
	//f.SetSize(16);
	//f.SetStyle(FontStyle::Bold);
	//f.SetStyle(FontStyle::Strikeout);
	//l->SetBorderStyle(BorderStyle::None);
	//l->SetFont(f);
	//
	button1 = window->AddButton("Abc", 200, 80, 50, 100);
	button1->OnClickSet([](Object* sender, EventArgs* e)
		{
			printf_s("Button1 Click Test");
		}
	);
	
	txtBox1 = window->AddTextBox("TextBox", 400, 20, 400);
	
	////auto& mb = window->GetMenuStrip();
	////auto& item1 = mb.AddItem("Arquivo");
	////auto& item2 = mb.AddItem("Arquivo2");
	////auto& menu1 = mb.AddMenu("Submenu");
	////menu1.AddItem("Submenu-Item1").OnClickSet(&TestClick);
	////menu1.AddCheckItem("Check-test01", true).OnClickSet(&TestClick);
	////menu1.AddCheckItem("Check-test02", false).OnClickSet(&TestClick);
	////menu1.AddSeparator();
	////menu1.AddItemWithIcon("Submenu-Item2", "images\\1.bmp").OnClickSet(&TestClick);
	////menu1.AddSeparator();
	////auto& menu2 = menu1.AddMenu("Submenu-Item3");
	////menu2.AddItemWithIcon("Submenu2-Item1", "images\\2.bmp").OnClickSet(&TestClick);
	////menu2.AddSeparator();
	////menu2.AddRadioItem("Radio1", true).OnClickSet(&TestClick);
	////menu2.AddRadioItem("Radio2", true).OnClickSet(&TestClick);
	////menu2.AddRadioItem("Radio3", false).OnClickSet(&TestClick);
	////menu2.AddRadioItem("Radio4", true).OnClickSet(&TestClick);
	////menu2.AddSeparator();
	////menu2.AddRadioItem("Radio1", false).OnClickSet(&TestClick);
	////menu2.AddRadioItem("Radio2", true).OnClickSet(&TestClick);
	////menu1.AddItem("Submenu-Item4").OnClickSet(&TestClick);
	////menu1.AddItem("Submenu-Item5").OnClickSet(&TestClick);
	////window->UpdateMenuStrip();
	//
	////item1.OnClickSet(&TestClick);
	////item2.OnClickSet(&TestClick);
	////menu1.OnClickSet(&TestClick);
	////menu2.OnClickSet(&TestClick);
	//
	//button1 = &window->AddButton("Abc", 200, 80, 50, 100);
	//button1->OnClickSet(&TestClick);
	//button1->OnMouseEnterSet([](Object* sender, EventArgs* e)
	//	{
	//		printf_s("Teste Mouseenter Button1");
	//	}
	//);
	//button1->OnMouseLeaveSet([](Object* sender, EventArgs* e)
	//	{
	//		printf_s("Teste Mouseleave Button1");
	//	}
	//);
	//
	//button2 = &window->AddButton("XYZ", 100, 40, 200, 200);
	//button2->OnClickSet([](Object* sender, EventArgs* e)
	//	{
	//		Button* b = (Button*)sender;
	//		b->Dispose();
	//	}
	//);
	//button2->Font.SetStyle(FontStyle::Strikeout);
	//
	//txtBox1 = &window->AddTextBox("TextBox", 400, 20, 400);
	//txtBox1->BorderStyle = BorderStyle::None;
	//txtBox1->OnKeyPressSet([](Object* sender, KeyPressEventArgs* e)
	//	{
	//		TextBox* t = (TextBox*)sender;
	//		if (e->KeyChar == 'p')
	//		{
	//			t->Dispose();
	//		}
	//		//if (e->KeyChar == 'q')
	//		//{
	//	//		e->Handled = true;
	//	//		return;
	//		//}
	//	}
	//);
	//
	txtBox2 = window->AddTextBox("TextBox2", 400, 20, 460);
	txtBox2->BorderStyle = BorderStyle::FixedSingle;
	txtBox2->OnKeyPressSet([](Object* sender, KeyPressEventArgs* e)
		{
			TextBox* t = (TextBox*)sender;
			if (e->KeyChar == 'p')
			{
				t->Dispose();
			}
		}
	);
	//
	//txtBox3 = &window->AddTextBox("TextBox3", 400, 20, 560);
	//txtBox3->BorderStyle = BorderStyle::Fixed3D;
	//txtBox3->OnKeyPressSet([](Object* sender, KeyPressEventArgs* e)
	//	{
	//		TextBox* t = (TextBox*)sender;
	//		if (e->KeyChar == 'p')
	//		{
	//			t->Dispose();
	//		}
	//		//if (e->KeyChar == 'q')
	//		//{
	//	//		e->Handled = true;
	//	//		return;
	//		//}
	//	}
	//);
	//
	//progressBar = &window->AddProgressBar("abc", 300, 50, 400, 20);
}