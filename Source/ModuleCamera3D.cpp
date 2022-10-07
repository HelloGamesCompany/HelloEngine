#include "Headers.h"
#include <iostream>
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"

ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	CalculateViewMatrix();

	X = float3(1.0f, 0.0f, 0.0f);
	Y = float3(0.0f, 1.0f, 0.0f);
	Z = float3(0.0f, 0.0f, 1.0f);

	Position = float3(0.0f, 0.0f, 5.0f);
	Reference = float3(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");

	return true;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
UpdateStatus ModuleCamera3D::Update()
{
	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		std::cout << "Cam Pos: x(" << Position.x << ") y(" << Position.y << ") z(" << Position.z << ")" << std::endl;
	}

	//	// Implement a debug camera with keys and mouse
	//	// Now we can make this movememnt frame rate independant!

	float3 newPos(0, 0, 0);
	float speed = 3.0f * app->fps;
	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 10.0f * app->fps;

	//	if (app->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;
	//	if (app->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) newPos.y -= speed;

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
			/*X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = Rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = Rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));*/
		}

		if (dy != 0)
		{
			float DeltaY = math::DegToRad((float)dy * Sensitivity);

			math::Quat rotation;
			rotation.SetFromAxisAngle(X, DeltaY);

			Y = rotation * Y;
			Z = rotation * Z;

			/*Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);*/

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

	return UpdateStatus::UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const float3&Position, const float3&Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = (Position - Reference).Normalized();
	X = (float3(0.0f, 1.0f, 0.0f).Cross(Z)).Normalized();
	Y = Cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const float3&Spot)
{
	Reference = Spot;
	Z = (Position - Reference).Normalized();
	X = (float3(0.0f, 1.0f, 0.0f).Cross(Z)).Normalized();	
	Y = Cross(Z, X);
	// Z = normalize(Position - Reference);
	// X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	// Y = cross(Z, X);

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::Move(const float3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix.v[0][0];
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = float4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -Dot(X, Position), -Dot(Y, Position), -Dot(Z, Position), 1.0f);
	ViewMatrixInverse = ViewMatrix.Inverted();
}
