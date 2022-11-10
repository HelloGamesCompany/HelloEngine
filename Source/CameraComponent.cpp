#include "Headers.h"
#include "CameraComponent.h"
#include "ModuleCamera3D.h"
#include "CameraObject.h"
#include "GameObject.h"

CameraComponent::CameraComponent(GameObject* gameObject) : Component(gameObject)
{
	cameraObject = Application::Instance()->camera->CreateGameCamera();
	cameraObject->frameBuffer.SetBufferInfo();
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
		if (ImGui::Checkbox("Active##Camera", &auxiliaryBool))
		{
			auxiliaryBool ? Enable() : Disable();
		}

		int tempValue = cameraObject->FOV;
		if (ImGui::SliderInt("FOV", &tempValue, 20, 140))
		{
			cameraObject->SetFOV(tempValue);
		}

		ImGui::DragFloat("Near plane", &cameraObject->cameraFrustum.nearPlaneDistance, 0.1f, 0.01);
		ImGui::DragFloat("Far plane", &cameraObject->cameraFrustum.farPlaneDistance, 1.0f, 0.01);

		ImGui::TextColored(cameraObject->currentlyDisplaying ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1), cameraObject->currentlyDisplaying ? "Currently displaying" : "Not currently displaying");
		ImGui::SameLine();

		if (ImGui::Button("Set as drawing game camera."))
		{
			Application::Instance()->camera->SetCurrentActiveGameCamera(cameraObject);
		}

	}
}

void CameraComponent::OnEnable()
{
	cameraObject->active = true;
}

void CameraComponent::OnDisable()
{
	cameraObject->active = false;
}
