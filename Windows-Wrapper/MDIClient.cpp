#include "MDIClient.h"

CreateParams* MDIClient::CreateParameters()
{
    CreateParams* cp = Control::CreateParameters();

    cp->ClassName = "MDICLIENT";

    cp->Style |= WS_VSCROLL | WS_HSCROLL;
    cp->ExStyle |= WS_EX_CLIENTEDGE;
    cp->Param = new CLIENTCREATESTRUCT{ 0, 1 };  // This Param MUST be cleared to avoid leak!
    ISite* site = (Parent == nullptr) ? nullptr : Parent->GetSite();
    if(site != nullptr && site->IsDesignMode())
    {
        cp->Style |= WS_DISABLED;
        m_Enabled = false;
    }

    return cp;
}

MDIClient::MDIClient()
{
    SetStyle(ControlStyles::Selectable, false);
    m_BackgroundColor = Color::Black();
    //m_DockStyle = DockStyle::Fill;
}

const std::vector<Window*>& MDIClient::GetMDIChildren() const noexcept
{
    return m_Children;
}

void MDIClient::LayoutMDI(MDILayout value) noexcept
{
    if(GetHandle() == IntPtr::Zero())
        return;

    switch(value)
    {
        case MDILayout::Cascade:
            SendControlMessage(WM_MDICASCADE, 0, 0);
            break;
        case MDILayout::TileVertical:
            SendControlMessage(WM_MDITILE, MDITILE_VERTICAL, 0);
            break;
        case MDILayout::TileHorizontal:
            SendControlMessage(WM_MDITILE, MDITILE_HORIZONTAL, 0);
            break;
        case MDILayout::ArrangeIcons:
            SendControlMessage(WM_MDIICONARRANGE, 0, 0);
            break;
    }
}