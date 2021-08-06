#include "TextBox.h"

// Singleton WndClass
TextBox::TextBoxClass TextBox::TextBoxClass::m_TextBoxClass;

// Window class declarations
TextBox::TextBoxClass::TextBoxClass() noexcept
	:
	m_Instance(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = HandleMessageSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	RegisterClassEx(&wc);
}

TextBox::TextBoxClass::~TextBoxClass()
{
	UnregisterClass(m_ClassName, GetInstance());
}

const char* TextBox::TextBoxClass::GetName() noexcept
{
	return m_ClassName;
}

HINSTANCE TextBox::TextBoxClass::GetInstance() noexcept
{
	return m_TextBoxClass.m_Instance;
}

int TextBox::OnEraseBackground_Impl(HWND hwnd, HDC hdc) noexcept
{
    return 1;   // Returns 1 to avoid flickering
}

int TextBox::OnGetDLGCode(HWND hwnd, LPMSG msg) noexcept
{
	return DLGC_WANTALLKEYS | DLGC_WANTARROWS;
}

void TextBox::OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept
{
    switch (vk)
    {
    case 'V':
        if (0x8000 & GetKeyState(VK_CONTROL))
        {
            HANDLE h;
            char* cb;
            int len = 0, slen = 0;

            InputWndDelete(hwnd);

            OpenClipboard(NULL);
            h = GetClipboardData(CF_UNICODETEXT);

            // Paste data from clipboard
            cb = (char*)GlobalLock(h);

            if (cb)
            {
                Text.append(cb);
            }

            GlobalUnlock(h);
            CloseClipboard();
            InputWndRedraw(hwnd);
        }
        break;

    case VK_RIGHT:

        if (cursor - 1 >= MAXINPUTBUF || cursor >= Text.size())
            break;

        cursor++;

        if (!(GetKeyState(VK_SHIFT) & 0x8000))
            select = cursor;

        InputWndRedraw(hwnd);
        break;

    case VK_TAB:
        PostMessageW(GetParent(hwnd), WM_NEXTDLGCTL, GetKeyState(VK_SHIFT) & 0x8000, FALSE);
        break;

    case VK_LEFT:
        if (cursor <= 0)
            break;

        cursor--;

        if (!(GetKeyState(VK_SHIFT) & 0x8000))
            select = cursor;

        InputWndRedraw(hwnd);
        break;

    case VK_HOME:
        cursor = 0;

        if (!(GetKeyState(VK_SHIFT) & 0x8000))
            select = cursor;

        InputWndRedraw(hwnd);
        break;

    case VK_END:
        cursor = Text.size();

        if (!(GetKeyState(VK_SHIFT) & 0x8000))
            select = cursor;

        InputWndRedraw(hwnd);
        break;

    case VK_DELETE:
        if (cursor >= Text.size())
        {
            InputWndDelete(hwnd);
            InputWndRedraw(hwnd);
            break;
        }

        if (select == cursor)
            select++;

        InputWndDelete(hwnd);
        InputWndRedraw(hwnd);
        break;

    case VK_BACK:

        if (cursor <= 0)
        {
            InputWndDelete(hwnd);
            InputWndRedraw(hwnd);
            break;
        }


        if (select == cursor)
            cursor--;


        InputWndDelete(hwnd);
        InputWndRedraw(hwnd);
    }
}

void TextBox::OnKeyPressed_Impl(HWND hwnd, char c, int cRepeat) noexcept
{
    if (c < VK_SPACE)
        return;


    InputWndDelete(hwnd);

    if (Text.size() + 1 < MAXINPUTBUF)
    {
        Text.push_back(c);
        cursor++;
        select = cursor;
    }

    InputWndRedraw(hwnd);

}

void TextBox::OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept
{
	SetFocus(hwnd);
	PostMessageW(GetParent(hwnd), WM_NEXTDLGCTL, (WPARAM)hwnd, TRUE);
}

void TextBox::OnFocusEnter_Impl(HWND hwnd, HWND hwndOldFocus) noexcept
{
	RECT r;
	GetClientRect(hwnd, &r);
	// Create a solid black caret. 
	CreateCaret(hwnd, (HBITMAP)NULL, 2, r.bottom - r.top);

	ShowCaret(hwnd);
	InputWndRedraw(hwnd);
}

void TextBox::OnFocusLeave_Impl(HWND hwnd, HWND hwndNewFocus) noexcept
{
	HideCaret(hwnd);
	DestroyCaret();
}

void TextBox::OnPaint_Impl(HWND hWnd) noexcept
{
	HDC dc;
	PAINTSTRUCT paint;

	dc = BeginPaint(hWnd, &paint);
	InputWndDraw(hWnd, dc);
	EndPaint(hWnd, &paint);
}

TextBox::TextBox(Control* parent, const std::string& name, int width, int height, int x, int y)
	:
	WinControl(parent, name, width, height, x, y)
{
	Initialize();
}

TextBox::~TextBox()
{

}

void TextBox::Initialize() noexcept
{
	// Create window and get its handle
	Handle = CreateWindow(
		TextBoxClass::GetName(),																								// Class name
		Text.c_str(),																						// Window title
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPSIBLINGS | SS_LEFT | SS_NOTIFY,												// Style values
		Location.X,																							// X position
		Location.Y,																							// Y position
		Size.Width,																							// Width
		Size.Height,																						// Height
		static_cast<HWND>(Parent->Handle.ToPointer()),														// Parent handle
		(HMENU)GetId(),						                												// Menu handle
		TextBoxClass::GetInstance(),																			// Module instance handle
		this																								// Pointer to the button instance to work along with HandleMessageSetup function.
	);

	if (Handle.IsNull())
	{
		throw CTL_LAST_EXCEPT();
	}

    // Set the maximum Textbox size to 256
    Text.resize(256);
}

void TextBox::Disable() noexcept
{

}

void TextBox::Enable() noexcept
{

}

void TextBox::Hide()
{
	if (IsShown())
	{
		IsVisible = false;
		ShowWindow(static_cast<HWND>(Handle.ToPointer()), SW_HIDE);
	}
}

void TextBox::Show()
{
	if (!IsShown())
	{
		IsVisible = true;
		ShowWindow(static_cast<HWND>(Handle.ToPointer()), SW_SHOWDEFAULT);
	}
}