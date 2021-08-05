#pragma once

#include "Common.h"
#include "Control.h"

#include <optional>

template<typename T>
class IWinControl : public Control
{
protected:

	IWinControl(Control* parent, const std::string& text, int width, int height, int x, int y)
		:
		Control(parent, text, width, height, x, y)
	{}

	virtual ~IWinControl() = default;

	// Static functions which handle WinAPI messages to corresponding member function of the control
	static LRESULT WINAPI HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		// Use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
		if (msg == WM_NCCREATE)
		{
			// Extract pointer to window class from creation data
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			T* const pWnd = static_cast<T*>(pCreate->lpCreateParams);

			// Set WinAPI-managed user data do store pointer to window class
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

			// Set message function to normal (non-setup) handler now that setup is finished
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&IWinControl::HandleMessageForwarder));

			// Forward message to window class member function
			return pWnd->HandleMessage(hWnd, msg, wParam, lParam);
		}

		// If we get a message before the WM_NCCREATE message, handle with default handler
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	static LRESULT WINAPI HandleMessageForwarder(HWND hWnd, UINT msg, WPARAM lParam, LPARAM wParam) noexcept
	{
		// Retrieve pointer to window class
		T* const pWnd = reinterpret_cast<T*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		// Forward message to window class member function
		return pWnd->HandleMessage(hWnd, msg, lParam, wParam);
	}

	// Member function responsible to handle the messages of each different type of control 
	virtual LRESULT WINAPI HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept = 0;
};