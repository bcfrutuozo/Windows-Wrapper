#include "TestApplication.h"

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE pPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
{
#ifdef _DEBUG
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);

	_CrtMemState StartupMemState;
	_CrtMemCheckpoint(&StartupMemState);
	_CrtMemDumpStatistics(&StartupMemState);
#endif

	int ret = 0;

	try
	{
		SetProcessDPIAware();
		ret = TestApplication().Start();
	}
	catch (const std::exception& e)
	{
		ret = -1;
		MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		ret = -1;
		MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}

#ifdef _DEBUG
	_CrtMemState ShutdownMemState;
	_CrtMemCheckpoint(&ShutdownMemState);
	_CrtMemDumpStatistics(&ShutdownMemState);
	OutputDebugString("=============\n");
	_CrtMemDumpAllObjectsSince(&StartupMemState);
#endif

	return ret;
}