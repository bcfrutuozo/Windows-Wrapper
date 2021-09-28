#include "TestRealTimeApplication.h"
#include "Window.h"

void TestRealTimeApplication::HandleInput(float dt)
{
	//auto window = GetWindow();

//while (const auto& e = window->GetKeyboard().ReadKey())
//{
//	if (!e->IsPress())
//	{
//		switch (e->GetCode())
//		{
//		case VK_ESCAPE:
//			if (window->IsCursorEnabled())
//			{
//				window->DisableCursor();
//				window->GetMouse().EnableRaw();
//			}
//			else
//			{
//				window->EnableCursor();
//				window->GetMouse().DisableRaw();
//			}
//			break;
//		}
//	}
//}

//if (!window->IsCursorEnabled())
//{
//	if (window->GetKeyboard().IsKeyPressed('Y'))
//	{
//		
//	}

//	if (window->GetKeyboard().IsKeyPressed('R'))
//	{

//	}

//	if (window->GetKeyboard().IsKeyPressed('O'))
//	{

//	}

//	if (window->GetKeyboard().IsKeyPressed('P'))
//	{
//	}

//	if (window->GetKeyboard().IsKeyPressed('Q'))
//	{
//		/*Color x = m_Window->();
//		x.SetR(x.GetR() - 1);

//		if (x.GetR() >= 0) m_Window->SetForeColor(x);*/
//		//cameras->Translate({ 0.0f, 0.0f, dt });
//	}
//	if (window->GetKeyboard().IsKeyPressed('A'))
//	{
//		/*Color x = m_Window->GetForeColor();
//		x.SetG(x.GetG() - 1);

//		if (x.GetG() >= 0) m_Window->SetForeColor(x);*/
//		//cameras->Translate({ -dt, 0.0f, 0.0f });
//	}
//	if (window->GetKeyboard().IsKeyPressed('Z'))
//	{
//		/*Color x = m_Window->GetForeColor();
//		x.SetB(x.GetB() - 1);

//		if (x.GetB() >= 0) m_Window->SetForeColor(x);*/
//		//cameras->Translate({ 0.0f, 0.0f, -dt });
//	}
//	if (window->GetKeyboard().IsKeyPressed('W'))
//	{
//		/*Color x = m_Window->GetForeColor();
//		x.SetR(x.GetR() + 1);

//		if (x.GetR() < 256) m_Window->SetForeColor(x);*/

//		//cameras->Translate({ dt, 0.0f, 0.0f });
//	}
//	if (window->GetKeyboard().IsKeyPressed('S'))
//	{
//		/*Color x = m_Window->GetForeColor();
//		x.SetG(x.GetG() + 1);

//		if (x.GetG() < 256) m_Window->SetForeColor(x);*/

//		//cameras->Translate({ 0.0f, dt, 0.0f });
//	}
//	if (window->GetKeyboard().IsKeyPressed('X'))
//	{
//		/*Color x = m_Window->GetForeColor();
//		x.SetB(x.GetB() + 1);

//		if (x.GetB() < 256) m_Window->SetForeColor(x);*/

//		//cameras->Translate({ 0.0f, -dt, 0.0f });
//	}
//}


//while (const auto& delta = window->GetMouse().ReadRawDelta())
//{
//	if (!window->IsCursorEnabled())
//	{
//		//cameras->Rotate(static_cast<float>(delta->x), static_cast<float>(delta->y));
//	}
//}
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
	Window* window = new Window("Teste", 1920, 1080);
	Window* window2 = new Window("Oieee", 500, 500);
	window2->AddButton("Button2", 250, 250, 125, 125);

	//ToolStrip& p = window->AddToolStrip();
	//ToolStrip& j = window->AddToolStrip();
	//p.SetBackgroundColor(Color::Black());
	//j.SetBackgroundColor(Color::Blue());
	window->AddLabel("abcdef", 5, 100);

	window->GetMouse().EnableRaw();
	window->GetKeyboard().DisableAutorepeat();

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

	button1 = window->AddButton("Abc", 200, 80, 50, 100);
	button1->OnMouseEnterSet([](Object* sender, EventArgs* e)
		{
			printf_s("Teste Mouseenter Button1");
		}
	);
	button1->OnMouseLeaveSet([](Object* sender, EventArgs* e)
		{
			printf_s("Teste Mouseleave Button1");
		}
	);

	button2 = window->AddButton("XYZ", 100, 40, 200, 200);
	button2->OnClickSet([](Object* sender, EventArgs* e)
		{
			Button* b = (Button*)sender;
			b->Dispose();
		}
	);

	txtBox1 = window->AddTextBox("TextBox", 400, 20, 400);
	txtBox1->BorderStyle = BorderStyle::None;
	txtBox1->OnKeyPressSet([](Object* sender, KeyPressEventArgs* e)
		{
			TextBox* t = (TextBox*)sender;
			if (e->KeyChar == 'p')
			{
				t->Dispose();
			}
			//if (e->KeyChar == 'q')
			//{
		//		e->Handled = true;
		//		return;
			//}
		}
	);

	txtBox2 = window->AddTextBox("TextBox2", 400, 20, 460);
	txtBox2->BorderStyle = BorderStyle::FixedSingle;
	txtBox2->OnKeyPressSet([](Object* sender, KeyPressEventArgs* e)
		{
			TextBox* t = (TextBox*)sender;
			if (e->KeyChar == 'p')
			{
				t->Dispose();
			}
			//if (e->KeyChar == 'q')
			//{
		//		e->Handled = true;
		//		return;
			//}
		}
	);

	txtBox3 = window->AddTextBox("TextBox3", 400, 20, 560);
	txtBox3->BorderStyle = BorderStyle::Fixed3D;
	txtBox3->OnKeyPressSet([](Object* sender, KeyPressEventArgs* e)
		{
			TextBox* t = (TextBox*)sender;
			if (e->KeyChar == 'p')
			{
				t->Dispose();
			}
			//if (e->KeyChar == 'q')
			//{
		//		e->Handled = true;
		//		return;
			//}
		}
	);

	progressBar = window->AddProgressBar("abc", 300, 50, 400, 20);
}