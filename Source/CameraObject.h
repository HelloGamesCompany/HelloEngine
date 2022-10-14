#pragma once

#include "FrameBuffer.h"

class CameraObject
{
public:
	CameraObject();
	~CameraObject();

	void UpdateCameraInput();

	void Look(const float3& Position, const float3& Reference, bool RotateAroundReference = false);
	void LookAt(const float3& Spot);
	void Move(const float3& Movement);
	float* GetViewMatrix();

public:
	float3 X, Y, Z, Position, Reference;
	bool active = true;
	FrameBuffer frameBuffer;
private:
	void CalculateViewMatrix();

private:
	float4x4 ViewMatrix, ViewMatrixInverse;
	float3 offset;
	Application* app = nullptr;
};

