#pragma once

class IActivable
{
protected:

	// Set enabled as default
	bool Enabled = true;

public:

	virtual void Disable() noexcept = 0;
	virtual void Enable() noexcept = 0;
	inline bool IsEnabled() noexcept { return Enabled; }
};