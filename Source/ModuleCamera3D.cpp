#include "Headers.h"
#include <iostream>
#include "Application.h"
#include "ModuleCamera3D.h"

#include "ModuleWindow.h"
#include "GameObject.h"
#include "ModuleLayers.h"
#include "CameraComponent.h"
#include "SceneCameraObject.h"

ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	sceneCamera.frameBuffer.SetBufferInfo();
	sceneCamera.frameBuffer.SetDimensions(Application::Instance()->window->width, Application::Instance()->window->height);
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

CameraObject* ModuleCamera3D::CreateGameCamera()
{
	CameraObject* newCamera = new CameraObject();
	gameCameras.push_back(newCamera);
	if (gameCameras.size() == 1)
	{
		activeGameCamera = newCamera; // If this is the first game camera, assign the current display camera to this.
		newCamera->currentlyDisplaying = true;
	}
	return newCamera;
}

void ModuleCamera3D::SetCurrentActiveGameCamera(CameraObject* target)
{
	if (activeGameCamera != nullptr) activeGameCamera->currentlyDisplaying = false;
	activeGameCamera = target;
	target->currentlyDisplaying = true;
}

void ModuleCamera3D::EraseGameCamera(CameraObject* erasedCamera)
{
	// TODO: To be tested when deleting game objects is possible.
	/*for (int i = 0; i < gameCameras.size(); i++)
	{
		if (erasedCamera == gameCameras[i]) 
		{
			gameCameras.erase(gameCameras.begin() + i);
			if (activeGameCamera == erasedCamera)
			{
				activeGameCamera = gameCameras.empty() ? nullptr : gameCameras[0];
			}
			break;
		}
	}*/
	
}

// -----------------------------------------------------------------
UpdateStatus ModuleCamera3D::Update()
{
	if (frameBufferRegenCamera != nullptr)
	{
		frameBufferRegenCamera->RegenerateFrameBuffer(newBufferWidth, newBufferHeight);
		frameBufferRegenCamera = nullptr;
	}

	if (updateSceneCamera) sceneCamera.UpdateInput();

	return UpdateStatus::UPDATE_CONTINUE;
}

