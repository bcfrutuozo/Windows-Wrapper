#include "MenuStrip.h"
#include "MenuRoot.h"

MenuStrip::MenuStrip(Control* parent)
	:
	MenuRoot(parent, 0, 0)
{

};

void MenuStrip::Hide() noexcept
{
	if (IsShown())
	{
		if (SetMenu(static_cast<HWND>(Parent->Handle.ToPointer()), NULL) == 0)
		{
			throw;
		};

		IsVisible = false;
	}
}

void MenuStrip::Show() noexcept
{
	if (!IsShown())
	{
		if (SetMenu(static_cast<HWND>(Parent->Handle.ToPointer()), static_cast<HMENU>(Handle.ToPointer())) == 0)
		{
			throw;
		};

		IsVisible = true;
	}
}