#include "Headers.h"
#include "CameraComponent.h"
#include "ModuleCamera3D.h"
#include "CameraObject.h"
#include "GameObject.h"

CameraComponent::CameraComponent(GameObject* gameObject) : Component(gameObject)
{
	_type = Component::Type::CAMERA;
	cameraObject = Application::Instance()->camera->CreateGameCamera();
	cameraObject->frameBuffer.SetBufferInfo();
	_needsTransformCallback = true;
}

CameraComponent::~CameraComponent()
{
	RELEASE(cameraObject);
}

void CameraComponent::OnTransformCallback(float4x4 worldMatrix)
{
	cameraObject->cameraFrustum.pos = worldMatrix.TranslatePart();
	cameraObject->cameraFrustum.front = _gameObject->transform->GetForward();
	cameraObject->cameraFrustum.up = _gameObject->transform->GetUp();
}

#ifdef STANDALONE
void CameraComponent::OnEditor()
{
	bool created = true;
	if (ImGui::CollapsingHeader("Camera", &created, ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool auxiliaryBool = _isEnabled;
		if (ImGui::Checkbox("Active##Camera", &auxiliaryBool))
		{
			auxiliaryBool ? Enable() : Disable();
		}

		//if (ImGui::BeginCombo("Frustum type", _comboValues[_selectedCombo].c_str()))
		//{
		//	for (int i = 0; i < 2; i++)
		//	{
		//		bool selected = i == _selectedCombo;
		//		if (ImGui::Selectable(_comboValues[i].c_str(), &selected))
		//		{
		//			if (i == 0)
		//				cameraObject->ChangeToOrthograpic();
		//			else
		//				cameraObject->ChangeToPerspective();
		//			_selectedCombo = i;
		//		}
		//	}
		//	ImGui::EndCombo();
		//}

		ImGui::Checkbox("Culling", &cameraObject->isCullingActive);

		//if (_selectedCombo == 0)
		//	OrthographicEditorOptions();
		//else
		PerspectiveEditorOptions();


		ImGui::TextColored(cameraObject->currentlyDisplaying ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1), cameraObject->currentlyDisplaying ? "Currently displaying" : "Not currently displaying");
		ImGui::SameLine();
		if (ImGui::Button("Set as drawing game camera."))
		{
			Application::Instance()->camera->SetCurrentActiveGameCamera(cameraObject);
		}
	}
	if (!created)
		_gameObject->DestroyComponent(this);
}
#endif // STANDALONE

void CameraComponent::PerspectiveEditorOptions()
{
	int tempValue = cameraObject->FOV;
	if (ImGui::SliderInt("FOV", &tempValue, 20, 140))
	{
		cameraObject->SetFOV(tempValue);
	}

	ImGui::DragFloat("Near plane", &cameraObject->cameraFrustum.nearPlaneDistance, 0.1f, 0.01);
	ImGui::DragFloat("Far plane", &cameraObject->cameraFrustum.farPlaneDistance, 1.0f, 0.01);
}

void CameraComponent::OrthographicEditorOptions()
{
	float size = cameraObject->GetOrthographicSize();
	if (ImGui::DragFloat("Size", &size, 0.1f, 0.01f))
		cameraObject->ChangeOrthographicSize(size);
}

void CameraComponent::OnEnable()
{
	cameraObject->active = true;
}

void CameraComponent::OnDisable()
{
	cameraObject->active = false;
}

void CameraComponent::Serialization(json& j)
{
	json _j;

	_j["Type"] = _type;

	_j["Near plane"] = cameraObject->cameraFrustum.nearPlaneDistance;
	_j["Far plane"] = cameraObject->cameraFrustum.farPlaneDistance;
	_j["FOV"] = cameraObject->FOV;
	_j["Aspect ratio"] = cameraObject->aspectRatio;
	_j["Frustum type"] = cameraObject->cameraFrustum.type;
	_j["Orthographic size"] = cameraObject->_orthographicSize;
	_j["Active culling"] = cameraObject->isCullingActive;
	_j["Active camera"] = cameraObject->active;
	_j["Enabled"] = _isEnabled;

	j["Components"].push_back(_j);
}

void CameraComponent::DeSerialization(json& j)
{
	cameraObject->cameraFrustum.nearPlaneDistance = j["Near plane"];
	cameraObject->cameraFrustum.farPlaneDistance = j["Far plane"];
	cameraObject->ChangeAspectRatio(j["Aspect ratio"]);
	cameraObject->SetFOV(j["FOV"]);
	//cameraObject->cameraFrustum.type = j["Frustum type"];
	//cameraObject->ChangeOrthographicSize(j["Orthographic size"]);
	cameraObject->isCullingActive = j["Active culling"];
	cameraObject->active = j["Active camera"];

	if (cameraObject->active)
		Application::Instance()->camera->SetCurrentActiveGameCamera(cameraObject);

	bool enabled = j["Enabled"];
	if (!enabled)
		Disable();

	_gameObject->transform->ForceUpdate();
}