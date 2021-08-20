#include "MenuSeparator.h"

MenuSeparator::MenuSeparator(Menu* parent, unsigned int subitemIndex)
	:
	Menu(parent, "", subitemIndex, -1)	// Separator have section -1, as they are the section incrementer
{
	Initialize();
}

void MenuSeparator::Initialize()
{
	MENUITEMINFO mi = { 0 };
	mi.cbSize = sizeof(MENUITEMINFO);
	mi.fMask = MIIM_FTYPE;
	mi.fType = MFT_SEPARATOR;
	if (InsertMenuItem(static_cast<HMENU>(Parent->Handle.ToPointer()), Handle.ToInt32(), FALSE, &mi) == 0)
	{
		throw CTL_LAST_EXCEPT();
	}
}