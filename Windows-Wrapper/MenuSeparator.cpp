#include "MenuSeparator.h"

MenuSeparator::MenuSeparator(Menu* parent, unsigned int subitemIndex)
	:
	Menu(parent, subitemIndex)
{
	m_Id = 0;
}

MenuSeparator::~MenuSeparator()
{

}

void MenuSeparator::Bind()
{
	MENUITEMINFO mi = { 0 };
	mi.cbSize = sizeof(MENUITEMINFO);
	mi.fMask = MIIM_FTYPE;
	mi.fType = MFT_SEPARATOR;
	InsertMenuItem(static_cast<HMENU>(Parent->Handle.ToPointer()), (UINT)Handle.ToPointer(), FALSE, &mi);
}