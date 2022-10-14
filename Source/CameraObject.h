#pragma once

#include "FrameBuffer.h"
#include "glmath.h"

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
	float* GetProjectionMatrix();

	void RecalculateProjection();

	void RegenerateFrameBuffer(int width, int height);

	void ChangeAspectRatio(float aspectRatio);

public:
	float3 X, Y, Z, Position, Reference;
	bool active = true;
	FrameBuffer frameBuffer;
private:
	void CalculateViewMatrix();

private:
	float4x4 ViewMatrix, ViewMatrixInverse, ProjectionMatrix;
	Frustum cameraFrustum;
	float3 offset;
	Application* app = nullptr;
	bool changeFOVWithBufferSize;

	friend class ModuleCamera3D;
};

