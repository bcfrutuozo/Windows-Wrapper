#include "TestUserApplication.h"
#include "TestRealTimeApplication.h"

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  
#include <conio.h>
#endif

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE pPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
{
#ifdef _DEBUG
	FILE* fp;
	AllocConsole();
	freopen_s(&fp, "CONIN$", "r", stdin);
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONOUT$", "w", stderr);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
#endif

	try
	{
		SetProcessDPIAware();
		TestUserApplication::SetGraphicsType(GraphicsType::GDI);
		TestUserApplication::Run();
	}
	catch (const ControlException& e)
	{
		return -1;
		MessageBox(nullptr, e.GetExceptionMessage(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		return -1;
		MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}

#ifdef _DEBUG
	if (_CrtDumpMemoryLeaks() == TRUE)
	{
		printf_s("OHHHH NEIN... Memory leak was detected!");
	}
	else
	{
		printf_s("No leaks detected! Press any key to close the application...");
	}
	while (!_kbhit());
#endif

	return 0;
}