#pragma once

#include "ImWindow.h"

class ImWindowOpenGL : public ImWindow
{
public:
	ImWindowOpenGL();
	
	~ImWindowOpenGL();

	void Update() override;

private:
	// string = config name, bool = conifg value, int = config openGL tag
	std::map<std::string, std::pair<bool, int>> configs;

	ModuleRenderer3D* moduleRenderer = nullptr;
};