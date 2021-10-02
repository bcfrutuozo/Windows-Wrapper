#include "ComboBox.h"

void ComboBox::Draw(const Graphics& graphics, Drawing::Rectangle rectangle)
{
	auto hwnd = static_cast<HWND>(Handle.ToPointer());
	auto hdc = static_cast<HDC>(graphics.GetHDC().ToPointer());

	RECT rect;
	//
	//	Mask off the borders and draw ComboBox normally
	//
	GetClientRect(hwnd, &rect);

	InflateRect(&rect, -GetSystemMetrics(SM_CXEDGE), -GetSystemMetrics(SM_CYEDGE));

	rect.right -= GetSystemMetrics(SM_CXVSCROLL);

	IntersectClipRect(hdc, rect.left, rect.top, rect.right, rect.bottom);

	// Draw the ComboBox
	//HandleMessageForwarder(hwnd, WM_PAINT, (WPARAM)ps.hdc, 0);
	//CallWindowProc(OldComboProc, hwnd, msg, (WPARAM)ps.hdc, 0);

	//
	//	Now mask off inside and draw the borders
	//
	SelectClipRgn(hdc, NULL);
	rect.right += GetSystemMetrics(SM_CXVSCROLL);

	ExcludeClipRect(hdc, rect.left, rect.top, rect.right, rect.bottom);

	// draw borders
	RECT rect2;
	GetClientRect(hwnd, &rect2);
	FillRect(hdc, &rect2, GetSysColorBrush(COLOR_3DSHADOW));

	// now draw the button
	SelectClipRgn(hdc, NULL);
	rect.left = rect.right - GetSystemMetrics(SM_CXVSCROLL);

	if (m_OpenedControl == m_ChildWindow->Handle)
	{
		DrawFrameControl(hdc, &rect, DFC_SCROLL, DFCS_SCROLLCOMBOBOX | DFCS_FLAT | DFCS_PUSHED);
		//FillRect(hdc, &rect, GetSysColorBrush(COLOR_3DDKSHADOW));

		// SHOW LISTBOX
		if (!m_ChildWindow->IsShown())
		{
			m_ChildWindow->Show();
		}
	}
	else
	{
		DrawFrameControl(hdc, &rect, DFC_SCROLL, DFCS_SCROLLCOMBOBOX | DFCS_FLAT);
		//FillRect(hdc, &rect, GetSysColorBrush(COLOR_3DFACE));

		if (m_ChildWindow->IsShown())
		{
			m_ChildWindow->Hide();
		}
	}


	DrawText(hdc, GetSelectedValue().c_str(), -1, &rect, DT_LEFT | DT_VCENTER);
	DrawText(hdc, GetSelectedValue().c_str(), static_cast<int>(GetSelectedValue().length()), &rect, DT_LEFT | DT_VCENTER | DT_CALCRECT);
}

void ComboBox::OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags)
{
	RECT rect{ 0 };

	InflateRect(&rect, -GetSystemMetrics(SM_CXEDGE), -GetSystemMetrics(SM_CYEDGE));
	rect.left = rect.right - GetSystemMetrics(SM_CXVSCROLL);

	WinAPI::OnMouseLeftDown_Impl(hwnd, x, y, keyFlags);

	// Sets the current opened control after calling WinAPI::OnMouseLeftDown_Impl which will first clear it.
	m_OpenedControl = m_ChildWindow->Handle;
}

ComboBox::ComboBox(Control* parent, const std::string& name, int width, int x, int y)
	:
	ListControl(parent, name, width, 0, x, y),	// Default control size without font is 9
	m_FlatStyle(FlatStyle::Standard_Windows10)
{
	// The ComboBox list MUST be inserted on the PARENT. Othewise it will be set in an invalid rectangle of the ComboBox
	m_ChildWindow = new ComboBoxChildNativeWindow(parent, this, m_Size.Width, 120, m_Location.X, m_Location.Y + m_Size.Height + 5);
	Controls.push_back(m_ChildWindow);
}

ComboBox::~ComboBox()
{

}

void ComboBox::ComboBoxChildNativeWindow::OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags)
{
}

void ComboBox::ComboBoxChildNativeWindow::OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags)
{
}

void ComboBox::ComboBoxChildNativeWindow::OnMouseLeftUp_Impl(HWND hwnd, int x, int y, unsigned int keyFlags)
{
	if (m_OpenedControl == Handle)
	{
		m_OpenedControl = nullptr;
		Hide();

		m_ComboBox->SetSelectedIndex(static_cast<int>(m_MouseOverIndex), true);
	}

	WinAPI::OnMouseLeftUp_Impl(hwnd, x, y, keyFlags);
}

void ComboBox::ComboBoxChildNativeWindow::OnMouseMove_Impl(HWND hwnd, int x, int y, unsigned int keyFlags)
{
	size_t i = 0;
	size_t oldMouseOver = m_MouseOverIndex;

	const auto drawableArea = GetDrawableArea();
	if (y >= drawableArea->top && y <= drawableArea->bottom && x >= drawableArea->left && x <= drawableArea->right)
	{
		for (auto it = m_RowPosition.begin(); it != m_RowPosition.end(); ++it, ++i)
		{
			if (y >= it->top && y <= it->bottom && x >= it->left && x <= it->right)
			{
				m_MouseOverIndex = i;
				break;
			}
		}
	}

	WinAPI::OnMouseMove_Impl(hwnd, x, y, keyFlags);

	// Redraw control only if MouseOverIndex changes to avoid CPU burn
	if (m_MouseOverIndex != oldMouseOver)
	{
		Update();
	}
}

ComboBox::ComboBoxChildNativeWindow::ComboBoxChildNativeWindow(Control* parent, ComboBox* comboBox, int width, int height, int x, int y)
	:
	ScrollableControl(parent, "", width, height, x, y),
	m_ComboBox(comboBox)
{
	Initialize();
}

ComboBox::ComboBoxChildNativeWindow::~ComboBoxChildNativeWindow()
{

}

void ComboBox::ComboBoxChildNativeWindow::Initialize()
{
	// Create window and get its handle
	Handle = CreateWindow(
		WindowClass::GetName(),									// Class name
		Text.c_str(),											// Window title
		WS_CHILD | WS_TABSTOP | WS_CLIPSIBLINGS,				// Style values
		m_Location.X,											// X position
		m_Location.Y,											// Y position
		m_Size.Width,											// Width
		m_Size.Height,											// Height
		static_cast<HWND>(Parent->Handle.ToPointer()),			// Parent handle
		nullptr,						                		// Menu handle
		WindowClass::GetInstance(),								// Module instance handle
		this													// Pointer to the class instance to work along with HandleMessageSetup function.
	);

	if (Handle.IsNull())
	{
		throw CTL_LAST_EXCEPT();
	}

	// Set default TextBox margin to 3 pixels
	m_BackgroundColor = Color::WindowBackground();

	// Initialize scrollbars after control creation
	HorizontalScrollBar.Initialize();
	VerticalScrollBar.Initialize();
}

void ComboBox::ComboBoxChildNativeWindow::PreDraw(const Graphics& graphics)
{
	// Load current font from default PreDraw function
	WinAPI::PreDraw(graphics);

	if (m_ComboBox->IsRebinding())
	{
		// Example test draw with the desired font to calculate each ListBox item size
		SIZE m_SingleSize;
		const char* verifier = "A";
		GetTextExtentPoint32(static_cast<HDC>(graphics.GetHDC().ToPointer()), verifier, 2, &m_SingleSize);
		SetMinimumItemWidth(m_SingleSize.cx);
		SetItemHeight(m_SingleSize.cy);

		int borderSize = 1;

		// Drawable block inside ListBox
		auto drawableArea = ResetDrawableArea();
		drawableArea->left += m_Margin.Left + borderSize;
		drawableArea->top += m_Margin.Top + borderSize;
		drawableArea->right -= m_Margin.Right + borderSize;
		drawableArea->bottom -= m_Margin.Bottom + borderSize;

		SetItemWidth(drawableArea->right - drawableArea->left);

		const auto& dataSource = m_ComboBox->GetDataSource();

		int itemsNumber = static_cast<int>(dataSource.size());

		// This block will only be executed once after resize
		m_RowPosition.clear();
		if (m_RowPosition.size() < static_cast<size_t>(itemsNumber))
		{
			m_RowPosition.resize(itemsNumber);
		}

		// Reset the amount of items in drawable area for recalculation
		m_TotalItemsInDrawableArea = 0;

		//int newWidth = 0;
		int newHeight = 0;
		//int oldWidth = static_cast<int>(drawableArea->right) - static_cast<int>(drawableArea->left);
		int oldHeight = static_cast<int>(drawableArea->bottom) - static_cast<int>(drawableArea->top);

		m_RowNumber = itemsNumber;

		if (oldHeight > (GetItemHeight() * itemsNumber))
		{
			VerticalScrollBar.SetMaximumValue(0);
			VerticalScrollBar.Hide();
		}
		else
		{
			while (newHeight < oldHeight)
			{
				newHeight += GetItemHeight();
			}

			drawableArea->right -= VerticalScrollBar.GetSize().Width;

			// Recalculate the new item width size
			SetItemWidth(drawableArea->right - drawableArea->left);

			drawableArea->bottom = newHeight + m_Margin.Top + (borderSize * 2);
			newHeight += m_Margin.Top + m_Margin.Bottom + (borderSize * 2);
			int oldHeightY = m_Size.Height;
			m_Size.Height = newHeight;
			auto vSize = VerticalScrollBar.GetSize();
			vSize.Height += (newHeight - oldHeightY);
			VerticalScrollBar.Resize(vSize);
			VerticalScrollBar.SetMaximumValue(itemsNumber);
			VerticalScrollBar.Show();
			Resize(m_Size.Width, newHeight + 1);

			int max = 0;
			while (max < newHeight)
			{
				max += GetItemHeight();

				if (max > newHeight)
				{
					break;
				}

				++m_TotalItemsInDrawableArea;
			}

			HandleMessageForwarder(static_cast<HWND>(VerticalScrollBar.Handle.ToPointer()), WM_SIZE, MAKEWPARAM(0, 0), MAKELPARAM(0, m_TotalItemsInDrawableArea));
		}

		m_ComboBox->m_IsRebinding = false;
	}
}

void ComboBox::ComboBoxChildNativeWindow::Draw(const Graphics& graphics, Drawing::Rectangle rectangle)
{
	HDC hdcMem;
	HBITMAP hbmMem;
	HBITMAP hbmOld;

	auto hwnd = static_cast<HWND>(Handle.ToPointer());
	auto hdc = static_cast<HDC>(graphics.GetHDC().ToPointer());

	auto drawableArea = GetDrawableArea();

	hdcMem = CreateCompatibleDC(hdc);
	hbmMem = CreateCompatibleBitmap(hdc, m_Size.Width, m_Size.Height);
	hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);

	RECT r;
	GetClientRect(hwnd, &r);
	HBRUSH bgColor = CreateSolidBrush(GetBackgroundColor().ToRGB());
	FillRect(hdcMem, &r, bgColor);
	SelectObject(hdcMem, bgColor);
	DeleteObject(bgColor);

	const auto& dataSource = m_ComboBox->GetDataSource();

	for (size_t i = 0; i < dataSource.size(); ++i)
	{
		RECT cr;
		CopyRect(&cr, drawableArea);

		cr.top = drawableArea->top - (GetItemHeight() * VerticalScrollBar.GetScrolling()) + (GetItemHeight() * static_cast<int>(i));
		cr.bottom = (cr.top + GetItemHeight());
		cr.left = drawableArea->left;
		cr.right = cr.left + GetItemWidth() - m_Margin.Right;

		m_RowPosition[i] = cr;

		if (cr.left >= drawableArea->left &&
			cr.top >= drawableArea->top &&
			cr.right <= drawableArea->right &&
			cr.bottom <= drawableArea->bottom)
		{
			if (GetMouseOverIndex() == i)
			{
				SetBkColor(hdcMem, RGB(0, 120, 215));
				SetTextColor(hdcMem, RGB(255, 255, 255));
				HBRUSH brush = CreateSolidBrush(RGB(0, 120, 215));
				FillRect(hdcMem, &cr, brush);
				SelectObject(hdcMem, brush);
				DeleteObject(brush);
			}
			else
			{
				SetBkColor(hdcMem, RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));
				SetTextColor(hdcMem, RGB(m_ForeColor.GetR(), m_ForeColor.GetG(), m_ForeColor.GetB()));
			}

			DrawText(hdcMem, dataSource[i].Value.c_str(), -1, &cr, DT_LEFT | DT_VCENTER);
			DrawText(hdcMem, Text.c_str(), static_cast<int>(dataSource[i].Value.length()), &cr, DT_LEFT | DT_VCENTER | DT_CALCRECT);
		}
	}

	// Perform the bit-block transfer between the memory Device Context which has the next bitmap
	// with the current image to avoid flickering

		BitBlt(hdc, 0, 0, m_Size.Width, m_Size.Height, hdcMem, 0, 0, SRCCOPY);

	SelectObject(hdcMem, hbmOld);
	DeleteObject(hbmOld);
	SelectObject(hdcMem, hbmOld);
	DeleteObject(hbmMem);
	ReleaseDC(hwnd, hdcMem);
	DeleteDC(hdcMem);
}

size_t ComboBox::ComboBoxChildNativeWindow::GetMouseOverIndex() const noexcept
{
	return m_MouseOverIndex;
}

void ComboBox::SetSelectedIndex(int index, bool value)
{
	m_SelectedIndex = index;

	if (m_SelectedIndex == -1)
	{
		m_SelectedValue = "";
	}
	else
	{
		Items[index].Selected = value;
		m_SelectedValue = Items[m_SelectedIndex].Value;
	}

	Update();
}

void ComboBox::SetSelectedValue(const ListItem& item)
{
	bool err = true;

	for (size_t i = 0; i < Items.size(); ++i)
	{
		if (Items[i].Id == item.Id && Items[i].Value == item.Value)
		{
			SetSelectedIndex(static_cast<int>(i), true);
			err = false;
			break;
		}
	}

	if (err)
	{
		throw ArgumentException("item");
	}
}