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

		void PreDraw(const Graphics& graphics) override;
		void Draw(const Graphics& graphics, Drawing::Rectangle rectangle) override;
		void OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) override;
		void OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) override;
		void OnMouseLeftUp_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) override;
		void OnMouseMove_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) override;

		ComboBoxChildNativeWindow(Control* parent, ComboBox* comboBox, int width, int height, int x, int y);
		virtual ~ComboBoxChildNativeWindow();

		void Initialize() override;
		size_t GetMouseOverIndex() const noexcept;
	};

	ComboBoxChildNativeWindow* m_ChildWindow;
	FlatStyle m_FlatStyle;

	void Draw(const Graphics& graphics, Drawing::Rectangle rectangle) override;
	void OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) override;

	ComboBox(Control* parent, const std::string& name, int width, int x, int y);

public:

	virtual ~ComboBox();

	void SetSelectedIndex(int index, bool value) override;
	void SetSelectedValue(const ListItem& item) override;
};
