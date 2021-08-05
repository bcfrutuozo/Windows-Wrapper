#pragma once

#include "EventArgs.h"
#include "Keys.h"

class KeyEventArgs : public EventArgs
{
public:

    //
    // Summary:
    //     Gets a value indicating whether the ALT key was pressed.
    //
    // Returns:
    //     true if the ALT key was pressed; otherwise, false.
    bool Alt;
    //
    // Summary:
    //     Gets a value indicating whether the CTRL key was pressed.
    //
    // Returns:
    //     true if the CTRL key was pressed; otherwise, false.
    bool Control;
    //
    // Summary:
    //     Gets or sets a value indicating whether the event was handled.
    //
    // Returns:
    //     true to bypass the control's default handling; otherwise, false to also pass
    //     the event along to the default control handler.
    bool Handled;
    //
    // Summary:
    //     Gets the keyboard code for a System.Windows.Forms.Control.KeyDown or System.Windows.Forms.Control.KeyUp
    //     event.
    //
    // Returns:
    //     A System.Windows.Forms.Keys value that is the key code for the event.
    Keys KeyCode;
    //
    // Summary:
    //     Gets the keyboard value for a System.Windows.Forms.Control.KeyDown or System.Windows.Forms.Control.KeyUp
    //     event.
    //
    // Returns:
    //     The integer representation of the System.Windows.Forms.KeyEventArgs.KeyCode property.
    int KeyValue;
    //
    // Summary:
    //     Gets the key data for a System.Windows.Forms.Control.KeyDown or System.Windows.Forms.Control.KeyUp
    //     event.
    //
    // Returns:
    //     A System.Windows.Forms.Keys representing the key code for the key that was pressed,
    //     combined with modifier flags that indicate which combination of CTRL, SHIFT,
    //     and ALT keys was pressed at the same time.
    Keys KeyData;
    //
    // Summary:
    //     Gets the modifier flags for a System.Windows.Forms.Control.KeyDown or System.Windows.Forms.Control.KeyUp
    //     event. The flags indicate which combination of CTRL, SHIFT, and ALT keys was
    //     pressed.
    //
    // Returns:
    //     A System.Windows.Forms.Keys value representing one or more modifier flags.
    Keys Modifiers;
    //
    // Summary:
    //     Gets a value indicating whether the SHIFT key was pressed.
    //
    // Returns:
    //     true if the SHIFT key was pressed; otherwise, false.
    bool Shift;
    //
    // Summary:
    //     Gets or sets a value indicating whether the key event should be passed on to
    //     the underlying control.
    //
    // Returns:
    //     true if the key event should not be sent to the control; otherwise, false.
    bool SuppressKeyPress;

	KeyEventArgs(Keys keyData)
        :
        Alt((keyData& Keys::Alt) == Keys::Alt),
        Control((keyData& Keys::Control) == Keys::Control),
        Handled(false),
        KeyCode((Keys)(keyData& Keys::KeyCode)),
        KeyData(keyData),
        KeyValue(static_cast<int>(keyData& Keys::KeyCode)),
        Modifiers((Keys)(keyData& Keys::Modifiers)),
        Shift((keyData& Keys::Shift) == Keys::Shift),
        SuppressKeyPress(false)
	{

    }
};