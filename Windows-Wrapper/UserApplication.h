#pragma once

#include "Application.h"

class UserApplication : public Application
{
public:

	UserApplication();
	~UserApplication();
	UserApplication(const UserApplication&) = delete;				// Copy constructor
	UserApplication(UserApplication&&) = delete;					// Move constructor
	UserApplication& operator=(const UserApplication&) = delete;	// Copy assignment constructor
	UserApplication& operator=(UserApplication&&) = delete;			// Move assignment constructor

	const int Start() override;
};
