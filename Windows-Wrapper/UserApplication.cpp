#include "UserApplication.h"

UserApplication::UserApplication()
{

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

	return 0;
}