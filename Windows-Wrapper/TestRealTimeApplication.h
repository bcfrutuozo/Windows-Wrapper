#pragma once

#include "RealTimeApplication.h"

class TestRealTimeApplication : public RealTimeApplication<TestRealTimeApplication>
{
private:

	Window* MainWindow;

public:

	void HandleInput(float dt) override;
	void Process(float dt) override;
	void Initialize() override;
};

