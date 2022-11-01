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
	LOG("Setting up the camera");

	// Create Empty GameObject with camera component.
	GameObject* mainCamera = new GameObject(Application::Instance()->layers->rootGameObject, "Main Camera", "Camera");
	CameraComponent* camera = mainCamera->AddComponent<CameraComponent>();
	currentDrawingCamera = activeGameCamera = camera->cameraObject = new CameraObject();

	sceneCamera.frameBuffer.SetBufferInfo();
	sceneCamera.frameBuffer.SetDimensions(Application::Instance()->window->width, Application::Instance()->window->height);

	activeGameCamera->frameBuffer.SetBufferInfo();
	activeGameCamera->frameBuffer.SetBufferInfo();

	/*for (auto& camera : gameCameras)
	{
		camera.second.frameBuffer.SetBufferInfo();
		camera.second.frameBuffer.SetDimensions(Application::Instance()->window->width, Application::Instance()->window->height);
	}*/

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

	if (updateSceneCamera) sceneCamera.UpdateInput();

	return UpdateStatus::UPDATE_CONTINUE;
}

