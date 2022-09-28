#pragma once

#include "ImWindow.h"

class ImWindowOpenGL : public ImWindow
{
public:
	ImWindowOpenGL();
	
	void Update() override;

private:
	bool depthTest;
	bool cullFace;
	bool lighting;
	bool colorMaterial;
	bool texture2D;
	bool blend;
	bool wireframe;

	ModuleRenderer3D* moduleRenderer = nullptr;
};