#pragma once

#include "FrameBuffer.h"
#include "Shader.h"

enum class CameraType
{
	SCENE,
	GAME,
};


class CameraObject
{
public:
	CameraObject();
	virtual ~CameraObject();

	void Look(const float3& Position, const float3& Reference, bool RotateAroundReference = false);
	void LookAt(const float3& Spot);
	void Move(const float3& Movement);
	
	void DrawFrustum();
	void SetUpBuffers();

	float* GetViewMatrix();
	float* GetProjectionMatrix();

	float* GetViewMatrixNoTransp();
	float* GetProjectionMatrixNoTransp();

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
	float4x4 ViewMatrix = float4x4::identity, ViewMatrixInverse = float4x4::identity, ProjectionMatrix = float4x4::identity;

	float3 offset = { 0,0,0 };
	float aspectRatio = 0.0f;
	float FOV = 60.0f;

	std::vector<uint> boxIndices;

	Shader* localLineShader = nullptr;
	uint VAO = 0;
	uint VBO = 0;
	uint IBO = 0;

	friend class ModuleCamera3D;
	friend class CameraComponent;

protected:
	Application* app = nullptr;


};

