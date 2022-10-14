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

	Position = float3(0.0f, 0.0f, 5.0f);
	Reference = float3(0.0f, 0.0f, 0.0f);

	ProjectionMatrix.type = math::FrustumType::PerspectiveFrustum;

	ProjectionMatrix.horizontalFov = Application::Instance()->window->width;
	ProjectionMatrix.verticalFov = Application::Instance()->window->height;

	ProjectionMatrix.nearPlaneDistance = 0.01f;
	ProjectionMatrix.farPlaneDistance = 1000.0f;

	ProjectionMatrix.pos = Position;
	ProjectionMatrix.front = Z;
	ProjectionMatrix.up = Y;

	tempProjMatrix = perspective(60, 1280 / 720, 0.01f, 100.0f);
}

CameraObject::~CameraObject()
{
}

void CameraObject::UpdateCameraInput()
{
	//	// Implement a debug camera with keys and mouse
	//	// Now we can make this movememnt frame rate independant!

	float3 newPos(0, 0, 0);
	float speed = 3.0f;
	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 10.0f * app->fps;

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

	Position += newPos;
	Reference += newPos;

	// Mouse motion ----------------

	if (app->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -app->input->GetMouseXMotion();
		int dy = -app->input->GetMouseYMotion();

		float Sensitivity = 0.25f;

		Position -= Reference;

		if (dx != 0)
		{
			float DeltaX = math::DegToRad((float)dx * Sensitivity);

			math::Quat rotation;
			rotation.SetFromAxisAngle({ 0.0f, 1.0f, 0.0f }, DeltaX);

			X = rotation * X;
			Y = rotation * Y;
			Z = rotation * Z;
		}

		if (dy != 0)
		{
			float DeltaY = math::DegToRad((float)dy * Sensitivity);

			math::Quat rotation;
			rotation.SetFromAxisAngle(X, DeltaY);

			Y = rotation * Y;
			Z = rotation * Z;

			if (Y.y < 0.0f)
			{
				Z = float3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = Cross(Z, X);
			}
		}

		Position = Reference + Z * Position.Length();
	}

	// Recalculate matrix -------------
	CalculateViewMatrix();
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
    return &ViewMatrix.v[0][0];
}

float* CameraObject::GetProjectionMatrix()
{
	ProjectionMatrix.ProjectionMatrix();
	tempProjMatrix.M;
	//float4x4 temp = ProjectionMatrix.ProjectionMatrix();
	//return &ProjectionMatrix.ProjectionMatrix()[0][0];
	return &tempProjMatrix.M[0];
}

void CameraObject::RecalculateProjection()
{
	tempProjMatrix = perspective(60, frameBuffer.width / frameBuffer.height, 0.01f, 100.0f);

	ProjectionMatrix.horizontalFov = frameBuffer.width;
	ProjectionMatrix.verticalFov = frameBuffer.height;

	ProjectionMatrix.nearPlaneDistance = 0.01f;
	ProjectionMatrix.farPlaneDistance = 1000.0f;
}

void CameraObject::RegenerateFrameBuffer(int width, int height)
{
	frameBuffer.SetDimensions(width, height);
	RecalculateProjection();
}

void CameraObject::CalculateViewMatrix()
{
    ViewMatrix = float4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -Dot(X, Position), -Dot(Y, Position), -Dot(Z, Position), 1.0f);
    ViewMatrixInverse = ViewMatrix.Inverted();
	ProjectionMatrix.pos = Position;
	ProjectionMatrix.front = Z;
	ProjectionMatrix.up = Y;
}
