#pragma once
#include "CameraObject.h"
class SceneCameraObject : public CameraObject
{
public:
	SceneCameraObject();
	~SceneCameraObject();

	void UpdateInput();

private:

	void Focus(const float3& focusPoint);
};

