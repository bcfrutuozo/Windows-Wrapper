#include "Application.h"

int CALLBACK WinMain(
	_In_ HINSTANCE cInstance,
	_In_opt_ HINSTANCE pInstance,
	_In_ LPSTR cmdLine,
	_In_ int cmdShow)
{
	try
	{

	}
	catch (const std::exception& e)
	{
		MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}

	return -1;
}