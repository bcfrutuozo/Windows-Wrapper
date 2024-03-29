#pragma once

#include "Control.h"
#include "Button.h"
#include "ProgressBar.h"
#include "TextBox.h"
#include "Label.h"

class ToolStrip: public Control
{
	friend class Control;

private:

	DockStyle m_DockStyle;
	ToolStripGripStyle m_GripStyle;
	ToolStripRenderMode m_Renderer;

	void Draw(Graphics* const graphics, Drawing::Rectangle rectangle);

protected:

	ToolStrip(Control* parent);

public:

	virtual ~ToolStrip() = default;

	void Initialize() override;

	DockStyle GetDockStyle() const noexcept;
	void SetDockStyle(DockStyle style) noexcept;
	ToolStripGripStyle GetGripStyle() const noexcept;
	void SetGripStyle(ToolStripGripStyle style) noexcept;
	ToolStripRenderMode GetRenderMode() const noexcept;
	void SetRenderMode(ToolStripRenderMode mode) noexcept;

	//Button& AddButton() noexcept;
	//ProgressBar& AddProgressBar() noexcept;
	//TextBox& AddTextBox() noexcept;
	//Label& AddLabel() noexcept;
};

