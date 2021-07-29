#pragma once

class IActivableMenu
{
protected:

	// Set enabled as default
	bool Enabled = true;
	bool Grayed = false;

public:

	virtual void Enable() noexcept = 0;
	virtual void Disable() noexcept = 0;
	virtual void Gray() noexcept = 0;

	bool IsEnabled() noexcept
	{
		return Enabled;
	}

	bool IsGrayed() noexcept
	{
		return Grayed;
	}
};