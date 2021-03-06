#pragma once

#include "Object.h"
#include "IntPtr.h"
#include "Map.h"

#include <string>
#include <array>
#include <Richedit.h>

struct Message;

using namespace std::literals::string_view_literals;

class MessageDecoder : public Object
{

#define REGISTERMAP(entry) { entry, #entry }

private:

	static constexpr std::array<std::pair<int, std::string_view>, 273> messages_array
	{
		{
			REGISTERMAP(WM_NULL),
			REGISTERMAP(WM_CREATE),
			REGISTERMAP(WM_DESTROY),
			REGISTERMAP(WM_MOVE),
			REGISTERMAP(WM_SIZE),
			REGISTERMAP(WM_ACTIVATE),
			REGISTERMAP(WA_INACTIVE),
			REGISTERMAP(WA_ACTIVE),
			REGISTERMAP(WA_CLICKACTIVE),
			REGISTERMAP(WM_SETFOCUS),
			REGISTERMAP(WM_KILLFOCUS),
			REGISTERMAP(WM_ENABLE),
			REGISTERMAP(WM_SETREDRAW),
			REGISTERMAP(WM_SETTEXT),
			REGISTERMAP(WM_GETTEXT),
			REGISTERMAP(WM_GETTEXTLENGTH),
			REGISTERMAP(WM_PAINT),
			REGISTERMAP(WM_CLOSE),
			REGISTERMAP(WM_QUERYENDSESSION),
			REGISTERMAP(WM_QUIT),
			REGISTERMAP(WM_QUERYOPEN),
			REGISTERMAP(WM_ERASEBKGND),
			REGISTERMAP(WM_SYSCOLORCHANGE),
			REGISTERMAP(WM_ENDSESSION),
			REGISTERMAP(WM_SHOWWINDOW),
			REGISTERMAP(WM_WININICHANGE),
			REGISTERMAP(WM_SETTINGCHANGE),
			REGISTERMAP(WM_DEVMODECHANGE),
			REGISTERMAP(WM_ACTIVATEAPP),
			REGISTERMAP(WM_FONTCHANGE),
			REGISTERMAP(WM_TIMECHANGE),
			REGISTERMAP(WM_CANCELMODE),
			REGISTERMAP(WM_SETCURSOR),
			REGISTERMAP(WM_MOUSEACTIVATE),
			REGISTERMAP(WM_CHILDACTIVATE),
			REGISTERMAP(WM_QUEUESYNC),
			REGISTERMAP(WM_GETMINMAXINFO),
			REGISTERMAP(WM_PAINTICON),
			REGISTERMAP(WM_ICONERASEBKGND),
			REGISTERMAP(WM_NEXTDLGCTL),
			REGISTERMAP(WM_SPOOLERSTATUS),
			REGISTERMAP(WM_DRAWITEM),
			REGISTERMAP(WM_MEASUREITEM),
			REGISTERMAP(WM_DELETEITEM),
			REGISTERMAP(WM_VKEYTOITEM),
			REGISTERMAP(WM_CHARTOITEM),
			REGISTERMAP(WM_SETFONT),
			REGISTERMAP(WM_GETFONT),
			REGISTERMAP(WM_SETHOTKEY),
			REGISTERMAP(WM_GETHOTKEY),
			REGISTERMAP(WM_QUERYDRAGICON),
			REGISTERMAP(WM_COMPAREITEM),
			REGISTERMAP(WM_GETOBJECT),
			REGISTERMAP(WM_COMPACTING),
			REGISTERMAP(WM_COMMNOTIFY),
			REGISTERMAP(WM_WINDOWPOSCHANGING),
			REGISTERMAP(WM_WINDOWPOSCHANGED),
			REGISTERMAP(WM_POWER),
			REGISTERMAP(WM_COPYDATA),
			REGISTERMAP(WM_CANCELJOURNAL),
			REGISTERMAP(WM_NOTIFY),
			REGISTERMAP(WM_INPUTLANGCHANGEREQUEST),
			REGISTERMAP(WM_INPUTLANGCHANGE),
			REGISTERMAP(WM_TCARD),
			REGISTERMAP(WM_HELP),
			REGISTERMAP(WM_USERCHANGED),
			REGISTERMAP(WM_NOTIFYFORMAT),
			REGISTERMAP(WM_CONTEXTMENU),
			REGISTERMAP(WM_STYLECHANGING),
			REGISTERMAP(WM_STYLECHANGED),
			REGISTERMAP(WM_DISPLAYCHANGE),
			REGISTERMAP(WM_GETICON),
			REGISTERMAP(WM_SETICON),
			REGISTERMAP(WM_NCCREATE),
			REGISTERMAP(WM_NCDESTROY),
			REGISTERMAP(WM_NCCALCSIZE),
			REGISTERMAP(WM_NCHITTEST),
			REGISTERMAP(WM_NCPAINT),
			REGISTERMAP(WM_NCACTIVATE),
			REGISTERMAP(WM_GETDLGCODE),
			REGISTERMAP(WM_NCMOUSEMOVE),
			REGISTERMAP(WM_NCLBUTTONDOWN),
			REGISTERMAP(WM_NCLBUTTONUP),
			REGISTERMAP(WM_NCLBUTTONDBLCLK),
			REGISTERMAP(WM_NCRBUTTONDOWN),
			REGISTERMAP(WM_NCRBUTTONUP),
			REGISTERMAP(WM_NCRBUTTONDBLCLK),
			REGISTERMAP(WM_NCMBUTTONDOWN),
			REGISTERMAP(WM_NCMBUTTONUP),
			REGISTERMAP(WM_NCMBUTTONDBLCLK),
			REGISTERMAP(WM_KEYFIRST),
			REGISTERMAP(WM_KEYDOWN),
			REGISTERMAP(WM_KEYUP),
			REGISTERMAP(WM_CHAR),
			REGISTERMAP(WM_DEADCHAR),
			REGISTERMAP(WM_SYSKEYDOWN),
			REGISTERMAP(WM_SYSKEYUP),
			REGISTERMAP(WM_SYSCHAR),
			REGISTERMAP(WM_SYSDEADCHAR),
			REGISTERMAP(WM_KEYLAST),
			REGISTERMAP(WM_IME_STARTCOMPOSITION),
			REGISTERMAP(WM_IME_ENDCOMPOSITION),
			REGISTERMAP(WM_IME_COMPOSITION),
			REGISTERMAP(WM_IME_KEYLAST),
			REGISTERMAP(WM_INITDIALOG),
			REGISTERMAP(WM_COMMAND),
			REGISTERMAP(WM_SYSCOMMAND),
			REGISTERMAP(WM_TIMER),
			REGISTERMAP(WM_HSCROLL),
			REGISTERMAP(WM_VSCROLL),
			REGISTERMAP(WM_INITMENU),
			REGISTERMAP(WM_INITMENUPOPUP),
			REGISTERMAP(WM_MENUSELECT),
			REGISTERMAP(WM_MENUCHAR),
			REGISTERMAP(WM_ENTERIDLE),
			REGISTERMAP(WM_CTLCOLORMSGBOX),
			REGISTERMAP(WM_CTLCOLOREDIT),
			REGISTERMAP(WM_CTLCOLORLISTBOX),
			REGISTERMAP(WM_CTLCOLORBTN),
			REGISTERMAP(WM_CTLCOLORDLG),
			REGISTERMAP(WM_CTLCOLORSCROLLBAR),
			REGISTERMAP(WM_CTLCOLORSTATIC),
			REGISTERMAP(WM_MOUSEFIRST),
			REGISTERMAP(WM_MOUSEMOVE),
			REGISTERMAP(WM_LBUTTONDOWN),
			REGISTERMAP(WM_LBUTTONUP),
			REGISTERMAP(WM_LBUTTONDBLCLK),
			REGISTERMAP(WM_RBUTTONDOWN),
			REGISTERMAP(WM_RBUTTONUP),
			REGISTERMAP(WM_RBUTTONDBLCLK),
			REGISTERMAP(WM_MBUTTONDOWN),
			REGISTERMAP(WM_MBUTTONUP),
			REGISTERMAP(WM_MBUTTONDBLCLK),
			REGISTERMAP(WM_MOUSEWHEEL),
			REGISTERMAP(WM_MOUSELAST),
			REGISTERMAP(WM_PARENTNOTIFY),
			REGISTERMAP(WM_ENTERMENULOOP),
			REGISTERMAP(WM_EXITMENULOOP),
			REGISTERMAP(WM_NEXTMENU),
			REGISTERMAP(WM_SIZING),
			REGISTERMAP(WM_CAPTURECHANGED),
			REGISTERMAP(WM_MOVING),
			REGISTERMAP(WM_POWERBROADCAST),
			REGISTERMAP(WM_DEVICECHANGE),
			REGISTERMAP(WM_IME_SETCONTEXT),
			REGISTERMAP(WM_IME_NOTIFY),
			REGISTERMAP(WM_IME_CONTROL),
			REGISTERMAP(WM_IME_COMPOSITIONFULL),
			REGISTERMAP(WM_IME_SELECT),
			REGISTERMAP(WM_IME_CHAR),
			REGISTERMAP(WM_IME_KEYDOWN),
			REGISTERMAP(WM_IME_KEYUP),
			REGISTERMAP(WM_MDICREATE),
			REGISTERMAP(WM_MDIDESTROY),
			REGISTERMAP(WM_MDIACTIVATE),
			REGISTERMAP(WM_MDIRESTORE),
			REGISTERMAP(WM_MDINEXT),
			REGISTERMAP(WM_MDIMAXIMIZE),
			REGISTERMAP(WM_MDITILE),
			REGISTERMAP(WM_MDICASCADE),
			REGISTERMAP(WM_MDIICONARRANGE),
			REGISTERMAP(WM_MDIGETACTIVE),
			REGISTERMAP(WM_MDISETMENU),
			REGISTERMAP(WM_ENTERSIZEMOVE),
			REGISTERMAP(WM_EXITSIZEMOVE),
			REGISTERMAP(WM_DROPFILES),
			REGISTERMAP(WM_MDIREFRESHMENU),
			REGISTERMAP(WM_MOUSEHOVER),
			REGISTERMAP(WM_MOUSELEAVE),
			REGISTERMAP(WM_CUT),
			REGISTERMAP(WM_COPY),
			REGISTERMAP(WM_PASTE),
			REGISTERMAP(WM_CLEAR),
			REGISTERMAP(WM_UNDO),
			REGISTERMAP(WM_RENDERFORMAT),
			REGISTERMAP(WM_RENDERALLFORMATS),
			REGISTERMAP(WM_DESTROYCLIPBOARD),
			REGISTERMAP(WM_DRAWCLIPBOARD),
			REGISTERMAP(WM_PAINTCLIPBOARD),
			REGISTERMAP(WM_VSCROLLCLIPBOARD),
			REGISTERMAP(WM_SIZECLIPBOARD),
			REGISTERMAP(WM_ASKCBFORMATNAME),
			REGISTERMAP(WM_CHANGECBCHAIN),
			REGISTERMAP(WM_HSCROLLCLIPBOARD),
			REGISTERMAP(WM_QUERYNEWPALETTE),
			REGISTERMAP(WM_PALETTEISCHANGING),
			REGISTERMAP(WM_PALETTECHANGED),
			REGISTERMAP(WM_HOTKEY),
			REGISTERMAP(WM_PRINT),
			REGISTERMAP(WM_PRINTCLIENT),
			REGISTERMAP(WM_HANDHELDFIRST),
			REGISTERMAP(WM_HANDHELDLAST),
			REGISTERMAP(WM_AFXFIRST),
			REGISTERMAP(WM_AFXLAST),
			REGISTERMAP(WM_PENWINFIRST),
			REGISTERMAP(WM_PENWINLAST),
			REGISTERMAP(WM_APP),
			REGISTERMAP(WM_USER),
			// REGISTERMAP(WM_CTLCOLOR),    // 16-bit Windows

			// RichEdit messages
			REGISTERMAP(WM_CONTEXTMENU),
			REGISTERMAP(WM_PRINTCLIENT),
			REGISTERMAP(EM_GETLIMITTEXT),
			REGISTERMAP(EM_POSFROMCHAR),
			REGISTERMAP(EM_CHARFROMPOS),
			REGISTERMAP(EM_SCROLLCARET),
			REGISTERMAP(EM_CANPASTE),
			REGISTERMAP(EM_DISPLAYBAND),
			REGISTERMAP(EM_EXGETSEL),
			REGISTERMAP(EM_EXLIMITTEXT),
			REGISTERMAP(EM_EXLINEFROMCHAR),
			REGISTERMAP(EM_EXSETSEL),
			REGISTERMAP(EM_FINDTEXT),
			REGISTERMAP(EM_FORMATRANGE),
			REGISTERMAP(EM_GETCHARFORMAT),
			REGISTERMAP(EM_GETEVENTMASK),
			REGISTERMAP(EM_GETOLEINTERFACE),
			REGISTERMAP(EM_GETPARAFORMAT),
			REGISTERMAP(EM_GETSELTEXT),
			REGISTERMAP(EM_HIDESELECTION),
			REGISTERMAP(EM_PASTESPECIAL),
			REGISTERMAP(EM_REQUESTRESIZE),
			REGISTERMAP(EM_SELECTIONTYPE),
			REGISTERMAP(EM_SETBKGNDCOLOR),
			REGISTERMAP(EM_SETCHARFORMAT),
			REGISTERMAP(EM_SETEVENTMASK),
			REGISTERMAP(EM_SETOLECALLBACK),
			REGISTERMAP(EM_SETPARAFORMAT),
			REGISTERMAP(EM_SETTARGETDEVICE),
			REGISTERMAP(EM_STREAMIN),
			REGISTERMAP(EM_STREAMOUT),
			REGISTERMAP(EM_GETTEXTRANGE),
			REGISTERMAP(EM_FINDWORDBREAK),
			REGISTERMAP(EM_SETOPTIONS),
			REGISTERMAP(EM_GETOPTIONS),
			REGISTERMAP(EM_FINDTEXTEX),
			REGISTERMAP(EM_GETWORDBREAKPROCEX),
			REGISTERMAP(EM_SETWORDBREAKPROCEX),
			// Richedit v2.0 messages
			REGISTERMAP(EM_SETUNDOLIMIT),
			REGISTERMAP(EM_REDO),
			REGISTERMAP(EM_CANREDO),
			REGISTERMAP(EM_GETUNDONAME),
			REGISTERMAP(EM_GETREDONAME),
			REGISTERMAP(EM_STOPGROUPTYPING),
			REGISTERMAP(EM_SETTEXTMODE),
			REGISTERMAP(EM_GETTEXTMODE),
			REGISTERMAP(EM_AUTOURLDETECT),
			REGISTERMAP(EM_GETAUTOURLDETECT),
			REGISTERMAP(EM_SETPALETTE),
			REGISTERMAP(EM_GETTEXTEX),
			REGISTERMAP(EM_GETTEXTLENGTHEX),
			// Asia specific messages
			REGISTERMAP(EM_SETPUNCTUATION),
			REGISTERMAP(EM_GETPUNCTUATION),
			REGISTERMAP(EM_SETWORDWRAPMODE),
			REGISTERMAP(EM_GETWORDWRAPMODE),
			REGISTERMAP(EM_SETIMECOLOR),
			REGISTERMAP(EM_GETIMECOLOR),
			REGISTERMAP(EM_SETIMEOPTIONS),
			REGISTERMAP(EM_GETIMEOPTIONS),
			REGISTERMAP(EM_CONVPOSITION),
			REGISTERMAP(EM_SETLANGOPTIONS),
			REGISTERMAP(EM_GETLANGOPTIONS),
			REGISTERMAP(EM_GETIMECOMPMODE),
			REGISTERMAP(EM_FINDTEXTW),
			REGISTERMAP(EM_FINDTEXTEXW),
			//Rich Edit 3.0 Asia msgs
			REGISTERMAP(EM_RECONVERSION),
			REGISTERMAP(EM_SETIMEMODEBIAS),
			REGISTERMAP(EM_GETIMEMODEBIAS),
			// BiDi Specific messages
			REGISTERMAP(EM_SETBIDIOPTIONS),
			REGISTERMAP(EM_GETBIDIOPTIONS),
			REGISTERMAP(EM_SETTYPOGRAPHYOPTIONS),
			REGISTERMAP(EM_GETTYPOGRAPHYOPTIONS),
			// Extended Edit style specific messages
			REGISTERMAP(EM_SETEDITSTYLE),
			REGISTERMAP(EM_GETEDITSTYLE),
		}
	};

	static std::string_view lookup_value(const int msg)
	{
		static constexpr auto map = Map<int, std::string_view, messages_array.size()>{ {messages_array} };
		return map.at(msg);
	}


	static constexpr std::string Parenthesize(const std::string input)
	{
		if (input.empty()) return "";
		else return "(" + input + ")";
	}

	static constexpr std::string MsgToString(int msg) noexcept
	{
		return std::string(lookup_value(msg));
	}

public:

	static std::string ToString(const Message* message) noexcept;
	static std::string ToString(IntPtr hWnd, int msg, IntPtr wParam, IntPtr lParam, IntPtr result) noexcept;
};