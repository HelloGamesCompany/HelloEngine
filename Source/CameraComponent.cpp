#include "Headers.h"
#include "CameraComponent.h"
#include "ModuleCamera3D.h"
#include "CameraObject.h"
#include "GameObject.h"

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
	moduleCameras->gameCameras[cameraID].cameraFrustum.pos = pos;
}

void CameraComponent::OnRotationUpdate(float3 rotation)
{
	moduleCameras->gameCameras[cameraID].cameraFrustum.front = _gameObject->transform->GetForward();
	moduleCameras->gameCameras[cameraID].cameraFrustum.up = _gameObject->transform->GetUp();
}

void CameraComponent::OnTransformUpdate(float3 pos, float3 scale, float3 rotation)
{
	moduleCameras->gameCameras[cameraID].cameraFrustum.pos = pos;
	moduleCameras->gameCameras[cameraID].cameraFrustum.front = _gameObject->transform->GetForward();
	moduleCameras->gameCameras[cameraID].cameraFrustum.up = _gameObject->transform->GetUp();
}