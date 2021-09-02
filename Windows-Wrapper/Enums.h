#pragma once

enum class CloseReason
{
	//
	// Summary:
	//     The cause of the closure was not defined or could not be determined.
	None = 0,
	//
	// Summary:
	//     The operating system is closing all applications before shutting down.
	WindowsShutDown = 1,
	//
	// Summary:
	//     The parent form of this multiple document interface (MDI) form is closing.
	MdiFormClosing = 2,
	//
	// Summary:
	//     The form is closing either programmatically or through a user action in the user
	//     interface (for example, by clicking the Close button on the form window, selecting
	//     Close from the window's control menu, or pressing ALT+F4).
	UserClosing = 3,
	//
	// Summary:
	//     The Microsoft Windows Task Manager is closing the application.
	TaskManagerClosing = 4,
	//
	// Summary:
	//     The owner form is closing.
	FormOwnerClosing = 5,
	//
	// Summary:
	//     The System.Windows.Forms.Application.Exit method of the System.Windows.Forms.Application
	//     class was invoked.
	ApplicationExitCall = 6
};


/* Specifies the border style for a control.
Documentation: https://docs.microsoft.com/en-us/dotnet/api/system.windows.forms.borderstyle?view=net-5.0*/
enum class BorderStyle
{
	None = 0,
	FixedSingle = 1,
	Fixed3D = 2
};

enum class GenericFontFamilies
{
	// A generic Serif FontFamily object.
	Serif = 0,

	// A generic Sans Serif FontFamily object.
	SansSerif = 1,

	// A generic Monospace FontFamily object.
	Monospace = 2
};

enum class FontStyle
{
	Regular = 0,
	Bold = 1,
	Italic = 2,
	Underline = 4,
	Strikeout = 8
};

enum class GraphicsUnit
{
	// Specifies the world coordinate system unit as the unit of measure.
	World = 0,

	// Specifies the unit of measure of the display device.Typically pixels for video displays, and 1 / 100 inch for printers.
	Display = 1,

	// Specifies a device pixel as the unit of measure.
	Pixel = 2,

	// Specifies a printer's point (1/72 inch) as the unit of measure.
	Point = 3,

	// Specifies the inch as the unit of measure.
	Inch = 4,

	// Specifies the document unit(1 / 300 inch) as the unit of measure.
	Document = 5,

	// Specifies the millimeter as the unit of measure.
	Millimeter = 6
};

enum class FlatStyle
{
	//The control appears flat.
	Flat = 0,

	//A control appears flat until the mouse pointer moves over it, at which point it appears three - dimensional.
	Popup = 1,

	//The control appears three - dimensional.
	Standard = 2,

	//The appearance of the control is determined by the user's operating system.
	System = 3
};

enum class ProgressBarAnimation
{
	Blocks = 0,
	Marquee = 1,
};

enum class DockStyle
{
	//
	// Summary:
	//     The control is not docked.
	None,
	//
	// Summary:
	//     The control's top edge is docked to the top of its containing control.
	Top,
	//
	// Summary:
	//     The control's bottom edge is docked to the bottom of its containing control.
	Bottom,
	//
	// Summary:
	//     The control's left edge is docked to the left edge of its containing control.
	Left,
	//
	// Summary:
	//     The control's right edge is docked to the right edge of its containing control.
	Right,
	//
	// Summary:
	//     All the control's edges are docked to the all edges of its containing control
	//     and sized appropriately.
	Fill
};

enum class ToolStripGripStyle
{
	//
	// Summary:
	//     Specifies that a System.Windows.Forms.ToolStrip move handle (grip) is not visible.
	Hidden,
	//
	// Summary:
	//     Specifies that a System.Windows.Forms.ToolStrip move handle (grip) is visible.
	Visible
};

enum class ToolStripGripDisplayStyle
{
	//
	// Summary:
	//     Specifies a horizontal orientation for the System.Windows.Forms.ToolStrip move
	//     handle (grip).
	Horizontal,
	//
	// Summary:
	//     Specifies a vertical orientation for the System.Windows.Forms.ToolStrip move
	//     handle (grip).
	Vertical
};

enum class ToolStripRenderMode
{
	//
	// Summary:
	//     Indicates the use of a System.Windows.Forms.ToolStripSystemRenderer to paint.
	System,
	//
	// Summary:
	//     Indicates the use of a System.Windows.Forms.ToolStripProfessionalRenderer to
	//     paint.
	Professional,
	//
	// Summary:
	//     Indicates that the System.Windows.Forms.ToolStripManager.RenderMode or System.Windows.Forms.ToolStripManager.Renderer
	//     determines the painting style.
	ManagerRenderMode
};

enum class SelectionMode
{
	None,
	Single,
	MultiSimple,
	MultiExtended
};