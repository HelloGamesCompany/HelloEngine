#pragma once
#include "CameraObject.h"

class LayerEditor;

class SceneCameraObject : public CameraObject
{
public:
	SceneCameraObject();
	~SceneCameraObject();

	void UpdateInput();

public:
	LayerEditor* editor = nullptr;

private:
	void Focus(const float3& focusPoint);

private:
	int cursorX, cursorY;
};

