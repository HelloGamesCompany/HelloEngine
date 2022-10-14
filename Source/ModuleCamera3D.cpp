#include "Headers.h"
#include <iostream>
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"

ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	gameCameras.emplace_back();
	currentDrawingCamera = activeGameCamera = &gameCameras[0];
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");

	sceneCamera.frameBuffer.SetBufferInfo();
	sceneCamera.frameBuffer.SetDimensions(Application::Instance()->window->width, Application::Instance()->window->height);
	sceneCamera.changeFOVWithBufferSize = true;

	for (int i = 0; i < gameCameras.size(); i++)
	{
		gameCameras[i].frameBuffer.SetBufferInfo();
		gameCameras[i].frameBuffer.SetDimensions(Application::Instance()->window->width, Application::Instance()->window->height);
		gameCameras[i].changeFOVWithBufferSize = false;
	}

	return true;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

void ModuleCamera3D::RequestFrameBufferRegen(CameraObject* camera, int width, int height)
{
	frameBufferRegenCamera = camera;
	newBufferWidth = width;
	newBufferHeight = height;
}

// -----------------------------------------------------------------
UpdateStatus ModuleCamera3D::Update()
{
	if (frameBufferRegenCamera != nullptr)
	{
		frameBufferRegenCamera->RegenerateFrameBuffer(newBufferWidth, newBufferHeight);
		frameBufferRegenCamera = nullptr;
	}

	if (updateSceneCamera) sceneCamera.UpdateCameraInput();
	if (updateGameCamera) activeGameCamera->UpdateCameraInput();

	return UpdateStatus::UPDATE_CONTINUE;
}

