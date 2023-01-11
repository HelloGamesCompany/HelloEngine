#pragma once
#include "ImWindow.h"
class ImWindowPerformanceTest : public ImWindow
{
public:
	ImWindowPerformanceTest();
	~ImWindowPerformanceTest();

	void Update() override;
};

