#include "UserControl.h"

CreateParams* UserControl::CreateParameters()
{
	CreateParams* cp = ContainerControl::CreateParameters();
	cp->ExStyle |= WS_EX_CONTROLPARENT;
	cp->ExStyle &= (~WS_EX_CLIENTEDGE);
	cp->Style &= (~WS_BORDER);

	switch(m_BorderStyle)
	{
		case BorderStyle::Fixed3D: cp->ExStyle |= WS_EX_CLIENTEDGE; break;
		case BorderStyle::FixedSingle: cp->Style |= WS_BORDER; break;
	}

	return cp;
}

UserControl::UserControl()
{
	SetScrollState(ScrollStateAutoScrolling, false);
	SetState(STATE_VISIBLE, true);
	SetState(STATE_TOPLEVEL, false);
	SetStyle(ControlStyles::SupportsTransparentBackColor, true);
}
