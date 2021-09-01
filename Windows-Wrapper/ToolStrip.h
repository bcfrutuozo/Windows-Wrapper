#pragma once

#include "Control.h"
#include "Enums.h"
#include "Button.h"
#include "ProgressBar.h"
#include "TextBox.h"
#include "Label.h"

class ToolStrip: public Control
{
private:

	DockStyle m_DockStyle;
	ToolStripGripStyle m_GripStyle;
	ToolStripRenderMode m_Renderer;

	// Singleton manages registration/cleanup of window class
	class ToolStripClass
	{
	private:

		static constexpr const char* m_ClassName = "ToolStrip Class";
		static ToolStripClass m_ToolStripClass;
		HINSTANCE m_Instance;

		ToolStripClass() noexcept;
		~ToolStripClass() noexcept;
		ToolStripClass(const ToolStripClass&) = delete;
		ToolStripClass& operator=(const ToolStripClass&) = delete;

	public:

		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	};

	void OnPaint_Impl(HWND hWnd) noexcept override;

protected:

public:

	ToolStrip(Control* parent);
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

