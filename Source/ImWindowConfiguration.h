#pragma once

#include <queue>

#include "ImWindow.h"

class Application;

class ImWindowConfiguration :public ImWindow
{
public:

	ImWindowConfiguration();

	~ImWindowConfiguration();

	void Update() override;

private:
	Application* app = nullptr;

	// Is not compatible with ImGui::PlotHistogram();
	std::queue<float> frames;

	std::vector<float> frames2;

	bool fullscreen = false;

	int countCPU = 0;

	float systemRAM = 0;

	int frameLimit = 60;
};