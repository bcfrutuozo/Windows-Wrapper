#pragma once

#include "UserApplication.h"

class TestUserApplication : public UserApplication<TestUserApplication>
{
private:

	//ProgressBar* progressBar = nullptr;
	//Button* button1 = nullptr;
	//Button* button2 = nullptr;

public:

	void Initialize() override;
};

