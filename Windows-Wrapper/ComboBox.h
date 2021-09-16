#pragma once

#include "ListControl.h"
#include "VerticalScrollBar.h"

class ComboBox : public ListControl
{
	friend class Control;

private:

	class ComboBoxChildNativeWindow : public ScrollableControl
	{
	public:

		ComboBox* m_ComboBox;
		std::vector<RECT> m_RowPosition;
		size_t m_TotalItemsInDrawableArea;
		size_t m_RowNumber;
		size_t m_MouseOverIndex;

		void OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept override;
		void OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept override;
		void OnMouseLeftUp_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept override;
		void OnMouseMove_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept override;
		void OnPaint_Impl(HWND hwnd) noexcept override;

		ComboBoxChildNativeWindow(Control* parent, ComboBox* comboBox, int width, int height, int x, int y);
		virtual ~ComboBoxChildNativeWindow();

		void Initialize() override;
		void Draw(HWND hwnd, HDC& hdc);
		void CalculateParameters(HWND hwnd, HDC& hdc);
		size_t GetMouseOverIndex() const noexcept;
	};

	ComboBoxChildNativeWindow* m_ChildWindow;
	FlatStyle m_FlatStyle;

	void OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept override;
	void OnPaint_Impl(HWND hwnd) noexcept override;

	ComboBox(Control* parent, const std::string& name, int width, int x, int y);

public:

	virtual ~ComboBox();

	void SetSelectedIndex(int index) noexcept override;
	void SetSelectedValue(const ListItem& item) override;
};

