#pragma once

// Not all WinApi controls are hidable, so it shouldn't be implemented on Base nor Control class
// An interface was created because each control has a particular way to show or hide.
class IHidable
{
protected:

	bool IsVisible = false;

public:

	virtual void Hide() = 0;
	inline bool IsShown() const noexcept { return IsVisible; }
	virtual void Show() = 0;
};