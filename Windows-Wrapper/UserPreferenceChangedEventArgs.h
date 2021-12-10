#pragma once

#include "EventArgs.h"
#include "UserPreferenceCategory.h"

class UserPreferenceChangedEventArgs : public EventArgs
{
public:

	const UserPreferenceCategory Category;

	UserPreferenceChangedEventArgs(UserPreferenceCategory category)
		:
		Category(category)
	{}
};