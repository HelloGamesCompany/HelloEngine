#pragma once

#include "ImWindow.h"

class ImWindowOpenGL : public ImWindow
{
public:
	ImWindowOpenGL();
	
	~ImWindowOpenGL();

	void Update() override;

private:
	//std::map<std::string, bool> configs;

	bool depthTest = false;
	bool cullFace = false;
	bool lighting = false;
	bool colorMaterial = false;
	bool texture2D = false;
	bool blend = false;
	bool wireframe = false;

	ModuleRenderer3D* moduleRenderer = nullptr;
};