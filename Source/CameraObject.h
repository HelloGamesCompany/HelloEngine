#pragma once

#include "FrameBuffer.h"

enum class CameraType
{
	SCENE,
	GAME,
};


class CameraObject
{
public:
	CameraObject();
	~CameraObject();

	void Look(const float3& Position, const float3& Reference, bool RotateAroundReference = false);
	void LookAt(const float3& Spot);
	void Move(const float3& Movement);
	
	float* GetViewMatrix();
	float* GetProjectionMatrix();

	void RecalculateProjection();

	void RegenerateFrameBuffer(int width, int height);

	void ChangeAspectRatio(float aspectRatio);

	float GetAspectRatio() { return aspectRatio; }

	void SetFOV(float fov);
	float GetFOV() { return FOV; };

	bool IsInsideFrustum(AABB& globalAABB);

public:
	float3 X = { 0,0,0 }, Y = { 0,0,0 }, Z = { 0,0,0 }, Position = { 0,0,0 }, Reference = { 0,0,0 };
	bool active = true;
	FrameBuffer frameBuffer;
	CameraType type = CameraType::GAME;
	Frustum cameraFrustum; // Should not be public, but we need ModuleCamera3D to access it.
	bool currentlyDisplaying = false;
	bool isCullingActive = true;


private:
	// TODO: Sospechoso de provocar excepcion en Release
	float4x4 ViewMatrix = float4x4::identity, ViewMatrixInverse = float4x4::identity, ProjectionMatrix = float4x4::identity;

	float3 offset = { 0,0,0 };
	float aspectRatio = 0.0f;
	float FOV = 60.0f;

	friend class ModuleCamera3D;
	friend class CameraComponent;

protected:
	Application* app = nullptr;


};

