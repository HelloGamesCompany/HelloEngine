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
	moduleCameras->gameCameras[cameraID].CalculateViewMatrix();
}

void CameraComponent::OnRotationUpdate(float3 rotation)
{
	float3 forward = _gameObject->transform->GetForward();
	float3 up = _gameObject->transform->GetUp();
	moduleCameras->gameCameras[cameraID].cameraFrustum.front = _gameObject->transform->GetForward();
	moduleCameras->gameCameras[cameraID].cameraFrustum.up = _gameObject->transform->GetUp();
	moduleCameras->gameCameras[cameraID].CalculateViewMatrix();
}

void CameraComponent::OnTransformUpdate(float3 pos, float3 scale, float3 rotation)
{
	moduleCameras->gameCameras[cameraID].cameraFrustum.pos = pos;
	moduleCameras->gameCameras[cameraID].cameraFrustum.front = _gameObject->transform->GetForward();
	moduleCameras->gameCameras[cameraID].cameraFrustum.up = _gameObject->transform->GetUp();
	moduleCameras->gameCameras[cameraID].CalculateViewMatrix();
}

CameraObject* CameraComponent::GetCameraObject()
{
	return &moduleCameras->gameCameras[cameraID]; 
}
