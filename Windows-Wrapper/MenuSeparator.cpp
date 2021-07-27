#include "MenuSeparator.h"

MenuSeparator::MenuSeparator(Menu* parent, unsigned int subitemIndex)
	:
	Menu(parent, "", subitemIndex, -1)	// Separator have section -1, as they are the section incrementer
{
	m_Id = 0;		// Override control Id because it doesn't trigger events
	Initialize();
}

void MenuSeparator::Initialize() noexcept
{
	MENUITEMINFO mi = { 0 };
	mi.cbSize = sizeof(MENUITEMINFO);
	mi.fMask = MIIM_FTYPE;
	mi.fType = MFT_SEPARATOR;
	InsertMenuItem(static_cast<HMENU>(Parent->Handle.ToPointer()), Handle.ToInt32(), FALSE, &mi);
}