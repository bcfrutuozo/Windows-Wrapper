#pragma once

#include "Control.h"
#include "Button.h"
#include "ProgressBar.h"
#include "TextBox.h"
#include "Label.h"

class ToolStrip final: public Control
{
private:

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

	//Button& AddButton() noexcept;
	//ProgressBar& AddProgressBar() noexcept;
	//TextBox& AddTextBox() noexcept;
	//Label& AddLabel() noexcept;
};

