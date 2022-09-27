#pragma once

#include <queue>

#include "ImWindow.h"

#include "ArrayQueue.hpp"

class Application;

class ImWindowConfiguration :public ImWindow
{
public:

	ImWindowConfiguration();

	~ImWindowConfiguration();

	void Update() override;

private:
	Application* app = nullptr;

	// Need more testing, but i think is good
	Htool::ArrayQueue<float>* frames = nullptr;

	//std::vector<float> frames2;

	bool fullscreen = false;

	int countCPU = 0;

	float systemRAM = 0;

	int* frameLimit = nullptr;

	const int* windowWidth = nullptr;
	const int* windowHeight = nullptr;

	float* windowBrightness = nullptr;

	bool* isVSyncOn = nullptr;
};