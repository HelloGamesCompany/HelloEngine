#include "Headers.h"
#include "CameraObject.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"

CameraObject::CameraObject()
{
	app = Application::Instance();

	X = float3(1.0f, 0.0f, 0.0f);
	Y = float3(0.0f, 1.0f, 0.0f);
	Z = float3(0.0f, 0.0f, 1.0f);

	Position = float3(0.0f, 0.0f, 0.0f);
	Reference = float3(0.0f, 0.0f, 0.0f);

	cameraFrustum.type = math::FrustumType::PerspectiveFrustum;
	FOV = 60;
	cameraFrustum.verticalFov = math::DegToRad(FOV);
	aspectRatio = 1.7f;
	cameraFrustum.horizontalFov = 2.0f * atanf(tanf(cameraFrustum.verticalFov / 2.0f) * aspectRatio);

	cameraFrustum.nearPlaneDistance = 0.01f;
	cameraFrustum.farPlaneDistance = 1000.0f;

	cameraFrustum.pos = Position;
	cameraFrustum.front = Z;
	cameraFrustum.up = Y;
}

CameraObject::~CameraObject()
{
	// TODO: Could be done more clean if we had an event system to commmunicate this kind of things.
	Application::Instance()->camera->EraseGameCamera(this);
}

void CameraObject::Look(const float3& Position, const float3& Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = (Position - Reference).Normalized();
	X = (float3(0.0f, 1.0f, 0.0f).Cross(Z)).Normalized();
	Y = Cross(Z, X);

	if (!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}
}

void CameraObject::LookAt(const float3& Spot)
{
	cameraFrustum.front = (Spot - cameraFrustum.pos).Normalized();
	float3 X = float3(0, 1, 0).Cross(cameraFrustum.front).Normalized();
	cameraFrustum.up = cameraFrustum.front.Cross(X);
}

void CameraObject::Move(const float3& Movement)
{
	cameraFrustum.pos += Movement;
}

float* CameraObject::GetViewMatrix()
{
	ViewMatrix = cameraFrustum.ViewMatrix();
	ViewMatrix.Transpose();
    return &ViewMatrix.v[0][0];
}

float* CameraObject::GetProjectionMatrix()
{
	ProjectionMatrix = cameraFrustum.ProjectionMatrix().Transposed();
	return &ProjectionMatrix.v[0][0];
}

void CameraObject::RecalculateProjection()
{
	float aspectRatio = (float)(frameBuffer.width) / (float)(frameBuffer.height);
	cameraFrustum.horizontalFov = 2.f * atanf(tanf(cameraFrustum.verticalFov * 0.5f) * aspectRatio);
}

void CameraObject::RegenerateFrameBuffer(int width, int height)
{
	frameBuffer.SetDimensions(width, height);
	RecalculateProjection();
}

void CameraObject::ChangeAspectRatio(float aspectRatio)
{
	this->aspectRatio = aspectRatio;
	cameraFrustum.horizontalFov = 2.0f * atanf(tanf(cameraFrustum.verticalFov * 0.5f) * aspectRatio);
}

void CameraObject::SetFOV(float fov)
{
	this->FOV = fov;
	cameraFrustum.verticalFov = math::DegToRad(fov);
	cameraFrustum.horizontalFov = 2.0f * atanf(tanf(cameraFrustum.verticalFov / 2.0f) * aspectRatio);
}

