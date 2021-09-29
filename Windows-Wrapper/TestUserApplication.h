#pragma once

#include "UserApplication.h"

class TestUserApplication : public UserApplication<TestUserApplication>
{
private:

	ProgressBar* progressBar = nullptr;
	Button* button1 = nullptr;
	Button* button2 = nullptr;
	TextBox* txtBox1 = nullptr;
	TextBox* txtBox2 = nullptr;
	TextBox* txtBox3 = nullptr;

public:

	void Initialize() override;
};

