# Windows-Wrapper
Win32 API wrapper to make our lives easier when using C++.

There's a lot of background work to deal with when instantiating a window (in Windows) and manage its Controls in C++.
The main goal of this project is to help a fellow programmer who wants to create a GUI for a C++ application but always struggles with it.

With a small mix of C# WinForms knowledge your life can be much easier with this framework. Yes, WinForms is not a new technology like WPF, WinUI, etc. However, it served as a main inspiration for the foundations of this project.

Now, let's get our hands dirty!

**First off, create a class which inherits UserApplication<T> (being T your class) or RealTimeApplication<T>, like the following one:**

```
class TestApplication : public UserApplication<TestApplication>
{
public:

	void Initialize() override;
};
```

This repository contains a TestApplication class like this one as an example.

**Now, just implements the Initialize() function by creating the application Windows and Controls:**
```
void TestApplication::Initialize()
{
	Window* window = new Window("Window1", 1920, 1080);
	Window* window2 = new Window("Window2", 500, 500);
	AddWindow(window);
	AddWindow(window2);

	button1 = &window->AddButton("Abc", 200, 80, 50, 100);
	button1->OnClickSet(&TestClick);
	button1->OnMouseEnterSet([](Object* sender, EventArgs* e)
		{
			OutputDebugString("Teste Mouseenter Button1");
		}
	);
  
	button2 = &window2->AddButton("XYZ", 100, 40, 200, 200);
	button2->OnClickSet([](Object* sender, EventArgs* e)
		{
			Button* b = (Button*)sender;
			b->Dispose();
		}
	);
	button2->Font.SetStyle(FontStyle::Strikeout);

	txtBox1 = &window->AddTextBox("TextBox", 400, 20, 400);
	txtBox1->BorderStyle = BorderStyle::None;
}

```
**And after it, you can create functions to perform many kind of events as it is in C#!:**
```
void TestClick(Object* sender, EventArgs* e)
{
	Button* b = (Button*)sender;
	b->Dispose();
}

void TestMouseMove(Object* sender, MouseEventHandler* e)
{
	int x = e->X;
  int y = e->Y;
}

```

**Finally, create the WinMain function to start your application and inside it, just call YOURCLASS::Run() function which will call the desired message loop according to your inherited class type:**
```
int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE pPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
{
	TestApplication::Run();
}
```

**Some important notes:**
  
  - UserApplication<T> calls the GetMessage() function to retrieves a message from the calling thread's message queue. The function dispatches incoming sent messages until a posted message is available for retrieval. (WAIS FOR EVENTS)
  - RealTimeApplication<T> calls the PeekMessage() function to dispatches incoming sent messages, checks the thread message queue for a posted message and retrieves the message (if any exist). (Removes the message from the Queue and DOES NOT WAIT FOR EVENTS).
  
**This framework is a hobbist project and I cannot be responsible for any harm it might cause.**
