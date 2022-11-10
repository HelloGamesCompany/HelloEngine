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

	//// Create Empty GameObject with camera component.
	//GameObject* mainCamera = new GameObject(Application::Instance()->layers->rootGameObject, "Main Camera", "Camera");
	//mainCamera->AddComponent<CameraComponent>();
	//activeGameCamera = mainCamera->GetComponent<CameraComponent>()->cameraObject;

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
	if (gameCameras.size() == 1) activeGameCamera = newCamera; // If this is the first game camera, assign the current display camera to this.
	return newCamera;
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

