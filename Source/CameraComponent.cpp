#include "Headers.h"
#include "CameraComponent.h"
#include "ModuleCamera3D.h"
#include "CameraObject.h"

CameraComponent::CameraComponent(GameObject* gameObject) : Component(gameObject)
{
	moduleCameras = Application::Instance()->camera;
	cameraID = moduleCameras->gameCameras.size();
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::OnPositionUpdate(float3 pos)
{
}

void CameraComponent::OnScaleUpdate(float3 scale)
{
}

void CameraComponent::OnRotationUpdate(float3 rotation)
{
}

void CameraComponent::OnTransformUpdate(float3 pos, float3 scale, float3 rotation)
{
}
