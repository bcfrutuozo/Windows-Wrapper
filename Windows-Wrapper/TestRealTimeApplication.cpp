#include "TestRealTimeApplication.h"
#include "Window.h"

void TestRealTimeApplication::HandleInput(float dt)
{
	while (const auto& e = MainWindow->GetKeyboard().ReadKey())
	{
		if (!e->IsPress())
		{
			switch (e->GetCode())
			{
			case VK_ESCAPE:
				if (MainWindow->IsCursorEnabled())
				{
					MainWindow->DisableCursor();
					MainWindow->GetMouse().EnableRaw();
				}
				else
				{
					MainWindow->EnableCursor();
					MainWindow->GetMouse().DisableRaw();
				}
				break;
			}
		}
	}

	if (!MainWindow->IsCursorEnabled())
	{
		if (MainWindow->GetKeyboard().IsKeyPressed('Y'))
		{
			std::ostringstream oss;
			oss << "Timer elapsed: " << std::setprecision(1) << std::fixed << dt << "s";
			printf_s(oss.str().c_str());
		}

		if (MainWindow->GetKeyboard().IsKeyPressed('R'))
		{

		}

		if (MainWindow->GetKeyboard().IsKeyPressed('O'))
		{

		}

		if (MainWindow->GetKeyboard().IsKeyPressed('P'))
		{
		}

		if (MainWindow->GetKeyboard().IsKeyPressed('Q'))
		{
			//cameras->Translate({ 0.0f, 0.0f, dt });
		}
	}

}

void TestRealTimeApplication::Process(float dt)
{
	// Time elapse on window title

	//const float t = m_Timer.Peek();
	std::ostringstream oss;
	oss << "Timer elapsed: " << std::setprecision(1) << std::fixed << dt << "s";
	Windows->front()->SetText(oss.str().c_str());

	// renderGraph.Execute(window.Gfx());

	// Present the frame
	//window.Gfx().EndFrame();
	//renderGraph.Reset();
}

void TestRealTimeApplication::Initialize()
{
	MainWindow = new Window("Teste", 1920, 1080);


	//ToolStrip& p = window->AddToolStrip();
	//ToolStrip& j = window->AddToolStrip();
	//p.SetBackgroundColor(Color::Black());
	//j.SetBackgroundColor(Color::Blue());
	MainWindow->AddLabel("abcdef", 5, 100);

	MainWindow->GetMouse().EnableRaw();
	MainWindow->GetKeyboard().DisableAutorepeat();

	//auto& mb = window->GetMenuStrip();
	//auto& item1 = mb.AddItem("Arquivo");
	//auto& item2 = mb.AddItem("Arquivo2");
	//auto& menu1 = mb.AddMenu("Submenu");
	//menu1.AddItem("Submenu-Item1").OnClickSet(&TestClick);
	//menu1.AddCheckItem("Check-test01", true).OnClickSet(&TestClick);
	//menu1.AddCheckItem("Check-test02", false).OnClickSet(&TestClick);
	//menu1.AddSeparator();
	//menu1.AddItemWithIcon("Submenu-Item2", "images\\1.bmp").OnClickSet(&TestClick);
	//menu1.AddSeparator();
	//auto& menu2 = menu1.AddMenu("Submenu-Item3");
	//menu2.AddItemWithIcon("Submenu2-Item1", "images\\2.bmp").OnClickSet(&TestClick);
	//menu2.AddSeparator();
	//menu2.AddRadioItem("Radio1", true).OnClickSet(&TestClick);
	//menu2.AddRadioItem("Radio2", true).OnClickSet(&TestClick);
	//menu2.AddRadioItem("Radio3", false).OnClickSet(&TestClick);
	//menu2.AddRadioItem("Radio4", true).OnClickSet(&TestClick);
	//menu2.AddSeparator();
	//menu2.AddRadioItem("Radio1", false).OnClickSet(&TestClick);
	//menu2.AddRadioItem("Radio2", true).OnClickSet(&TestClick);
	//menu1.AddItem("Submenu-Item4").OnClickSet(&TestClick);
	//menu1.AddItem("Submenu-Item5").OnClickSet(&TestClick);
	//window->UpdateMenuStrip();

	//item1.OnClickSet(&TestClick);
	//item2.OnClickSet(&TestClick);
	//menu1.OnClickSet(&TestClick);
	//menu2.OnClickSet(&TestClick);

	auto button1 = MainWindow->AddButton("Abc", 200, 80, 50, 100);
	button1->OnMouseEnterSet([](Object* sender, EventArgs* e)
		{
			std::ostringstream ossin;
			ossin << sender->ToString() << std::endl << e->ToString() << std::endl << std::endl;
			printf_s(ossin.str().c_str());
			printf_s("Teste Mouseenter Button1");
		}
	);
	button1->OnMouseLeaveSet([](Object* sender, EventArgs* e)
		{
			std::ostringstream ossin;
			ossin << sender->ToString() << std::endl << e->ToString() << std::endl << std::endl;
			printf_s(ossin.str().c_str());
			printf_s("Teste Mouseleave Button1");
		}
	);

	auto button2 = MainWindow->AddButton("XYZ", 100, 40, 200, 200);
	button2->OnClickSet([](Object* sender, EventArgs* e)
		{
			std::ostringstream ossin;
			ossin << sender->ToString() << std::endl << e->ToString() << std::endl << std::endl;
			printf_s(ossin.str().c_str());
		}
	);

	auto txtBox1 = MainWindow->AddTextBox("TextBox", 400, 20, 400);
	txtBox1->BorderStyle = BorderStyle::None;
	txtBox1->OnKeyPressSet([](Object* sender, KeyPressEventArgs* e)
		{
			std::ostringstream ossin;
			ossin << sender->ToString() << std::endl << e->ToString() << std::endl << std::endl;
			printf_s(ossin.str().c_str());
		}
	);

	auto txtBox2 = MainWindow->AddTextBox("TextBox2", 400, 20, 460);
	txtBox2->BorderStyle = BorderStyle::FixedSingle;
	txtBox2->OnKeyPressSet([](Object* sender, KeyPressEventArgs* e)
		{
			std::ostringstream ossin;
			ossin << sender->ToString() << std::endl << e->ToString() << std::endl << std::endl;
			printf_s(ossin.str().c_str());
		}
	);

	auto txtBox3 = MainWindow->AddTextBox("TextBox3", 400, 20, 560);
	txtBox3->BorderStyle = BorderStyle::Fixed3D;
	txtBox3->OnKeyPressSet([](Object* sender, KeyPressEventArgs* e)
		{
			std::ostringstream ossin;
			ossin << sender->ToString() << std::endl << e->ToString() << std::endl << std::endl;
			printf_s(ossin.str().c_str());
		}
	);

	MainWindow->AddProgressBar("abc", 300, 50, 400, 20);
}