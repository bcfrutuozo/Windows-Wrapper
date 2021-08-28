#pragma once

#include "UserApplication.h"

class TestApplication : public UserApplication<TestApplication>
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

