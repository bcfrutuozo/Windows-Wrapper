#pragma once

#include "RealTimeApplication.h"

class TestRealTimeApplication : public RealTimeApplication<TestRealTimeApplication>
{
private:

	ProgressBar* progressBar;
	Button* button1;
	Button* button2;
	TextBox* txtBox1;
	TextBox* txtBox2;
	TextBox* txtBox3;

public:

	void HandleInput(float dt) override;
	void Process(float dt) override;
	void Initialize() override;
};

