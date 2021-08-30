#pragma once

#include "UserApplication.h"

class TestUserApplication : public UserApplication<TestUserApplication>
{
private:

	ProgressBar* progressBar;
	Button* button1;
	Button* button2;
	TextBox* txtBox1;
	TextBox* txtBox2;
	TextBox* txtBox3;

public:

	void Initialize() override;
};

