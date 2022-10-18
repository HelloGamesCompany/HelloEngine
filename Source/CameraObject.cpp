#include "Headers.h"
#include "CameraObject.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"

CameraObject::CameraObject()
{
	app = Application::Instance();

	CalculateViewMatrix();

	X = float3(1.0f, 0.0f, 0.0f);
	Y = float3(0.0f, 1.0f, 0.0f);
	Z = float3(0.0f, 0.0f, 1.0f);

	Position = float3(0.0f, 0.0f, -5.0f);
	Reference = float3(0.0f, 0.0f, 0.0f);

	cameraFrustum.type = math::FrustumType::PerspectiveFrustum;
	cameraFrustum.verticalFov = math::DegToRad(60.0f);
	cameraFrustum.horizontalFov = 2.0f * atanf(tanf(cameraFrustum.verticalFov / 2.0f) * 1.7f);

	cameraFrustum.nearPlaneDistance = 0.01f;
	cameraFrustum.farPlaneDistance = 1000.0f;

	cameraFrustum.pos = Position;
	cameraFrustum.front = Z;
	cameraFrustum.up = Y;
}

CameraObject::~CameraObject()
{
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

    CalculateViewMatrix();
}

void CameraObject::LookAt(const float3& Spot)
{
    Reference = Spot;
    Z = (Position - Reference).Normalized();
    X = (float3(0.0f, 1.0f, 0.0f).Cross(Z)).Normalized();
    Y = Cross(Z, X);

    CalculateViewMatrix();
}

void CameraObject::Move(const float3& Movement)
{
    Position += Movement;
    Reference += Movement;

    CalculateViewMatrix();
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
	cameraFrustum.horizontalFov = 2.f * atanf(tanf(cameraFrustum.verticalFov * 0.5f) * aspectRatio);
}

void CameraObject::CalculateViewMatrix()
{
    /*ViewMatrix = float4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -Dot(X, Position), -Dot(Y, Position), -Dot(Z, Position), 1.0f);
    ViewMatrixInverse = ViewMatrix.Inverted();*/
	/*cameraFrustum.pos = Position;
	cameraFrustum.front = Z;
	cameraFrustum.up = Y;*/
}
