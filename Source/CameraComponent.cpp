#include "Headers.h"
#include "CameraComponent.h"
#include "ModuleCamera3D.h"
#include "CameraObject.h"
#include "GameObject.h"

CameraComponent::CameraComponent(GameObject* gameObject) : Component(gameObject)
{
	moduleCameras = Application::Instance()->camera;
	cameraID = moduleCameras->gameCameras.empty() ? 0 : moduleCameras->gameCameras.size();
}

CameraComponent::~CameraComponent()
{
	RELEASE(cameraObject);
}

void CameraComponent::OnPositionUpdate(float3 pos)
{
	cameraObject->cameraFrustum.pos = pos;
}

void CameraComponent::OnRotationUpdate(float3 rotation)
{
	float3 forward = _gameObject->transform->GetForward();
	float3 up = _gameObject->transform->GetUp();
	cameraObject->cameraFrustum.front = _gameObject->transform->GetForward();
	cameraObject->cameraFrustum.up = _gameObject->transform->GetUp();
}

void CameraComponent::OnTransformUpdate(float3 pos, float3 scale, float3 rotation)
{
	cameraObject->cameraFrustum.pos = pos;
	cameraObject->cameraFrustum.front = _gameObject->transform->GetForward();
	cameraObject->cameraFrustum.up = _gameObject->transform->GetUp();
}

void CameraComponent::OnEditor()
{
	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool auxiliaryBool = _isEnabled;
		if (ImGui::Checkbox("Active", &auxiliaryBool))
			Enable(auxiliaryBool, false);

		int tempValue = cameraObject->FOV;
		if (ImGui::SliderInt("FOV", &tempValue, 20, 140))
		{
			cameraObject->SetFOV(tempValue);
		}

		ImGui::DragFloat("Near plane", &cameraObject->cameraFrustum.nearPlaneDistance, 0.1f, 0.01);
		ImGui::DragFloat("Far plane", &cameraObject->cameraFrustum.farPlaneDistance, 1.0f, 0.01);

	}
}

void CameraComponent::Enable(bool enabled, bool fromGo)
{
	if (!fromGo) _isEnabled = enabled;
	moduleCameras->gameCameras[cameraID].active = enabled;

	if (_isEnabled && _gameObject->IsActive())
		moduleCameras->gameCameras[cameraID].active = true;
	else if (_isEnabled && !_gameObject->IsActive())
		moduleCameras->gameCameras[cameraID].active = false;
}

CameraObject* CameraComponent::GetCameraObject()
{
	return &moduleCameras->gameCameras[cameraID]; 
}
