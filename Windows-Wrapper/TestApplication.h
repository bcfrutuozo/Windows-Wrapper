#pragma once

#include "UserApplication.h"

class TestApplication : public UserApplication
{
private:

	ProgressBar* progressBar;
	Button* button1;
	Button* button2;
	TextBox* txtBox1;
	TextBox* txtBox2;
	TextBox* txtBox3;
	Menu* m1;
	Menu* m2;
	Menu* m3;

public:

	TestApplication()
	{
		Initialize();
	}

	virtual ~TestApplication()
	{

	}

	void Initialize() override;
};

