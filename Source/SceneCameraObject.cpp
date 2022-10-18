#include "Headers.h"
#include "SceneCameraObject.h"
#include "ModuleInput.h"

SceneCameraObject::SceneCameraObject()
{
}

SceneCameraObject::~SceneCameraObject()
{
}

void SceneCameraObject::UpdateInput()
{
	//	// Implement a debug camera with keys and mouse
	//	// Now we can make this movememnt frame rate independant!

	float3 newPos(0, 0, 0);
	float speed = 3.0f * app->fps;
	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 10.0f * app->fps;

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += cameraFrustum.front * speed;
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos -= cameraFrustum.front * speed;


	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= cameraFrustum.WorldRight() * speed;
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += cameraFrustum.WorldRight() * speed;

	Position += newPos;
	Reference += newPos;

	cameraFrustum.pos += newPos;

	// Mouse motion ----------------

	if (app->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -app->input->GetMouseXMotion();
		int dy = -app->input->GetMouseYMotion();

		float Sensitivity = 0.25f;

		float3 empty = { 0,0,0 };
		Quat lookingDir = Quat::identity;
		cameraFrustum.WorldMatrix().Decompose(empty, lookingDir, empty);

		if (dy != 0)
		{
			float DeltaY = math::DegToRad((float)dy * Sensitivity);

			math::Quat rotation = Quat::identity;
			rotation.SetFromAxisAngle(float3(1, 0, 0), DeltaY);

			lookingDir = lookingDir * rotation;
		}

		if (dx != 0)
		{
			float DeltaX = math::DegToRad((float)dx * Sensitivity);

			math::Quat rotation = Quat::identity;
			rotation.SetFromAxisAngle({ 0.0f, 1.0f, 0.0f }, DeltaX);

			lookingDir = rotation * lookingDir;
		}

		float4x4 newWorldMatrix = cameraFrustum.WorldMatrix();
		newWorldMatrix.SetRotatePart(lookingDir.Normalized());
		cameraFrustum.SetWorldMatrix(newWorldMatrix.Float3x4Part());
	}
	
}
