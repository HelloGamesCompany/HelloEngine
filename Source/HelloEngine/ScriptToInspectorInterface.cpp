#include "Headers.h"
#include "ScriptToInspectorInterface.h"
#include "ModuleLayers.h"
#include "API/API_Transform.h"
#include "API/API_MeshRenderer.h"
#include "API/API_Camera.h"
#include "API/API_RigidBody.h"
#include "API/API_AnimationPlayer.h"
#include "API/API_UIButton.h"
#include "API/API_UIImage.h"
#include "PhysicsComponent.h"
#include "MeshRenderComponent.h"
#include "CameraComponent.h"
#include "AnimationComponent.h"
#include "ComponentUIButton.h"
#include "ComponentUIImage.h"

void DragFieldFloat::OnEditor()
{
	ImGui::DragFloat((valueName + "##" + className).c_str(), (float*)value);
}

void DragFieldFloat::OnSerialize(json& j)
{
	json _j;
	_j[valueName.c_str()] = *(float*)value;
	j.push_back(_j);
}

void DragFieldFloat::OnDeserialize(json& j)
{
	for (int i = 0; i < j.size(); i++)
	{
		if (j[i].find(valueName) != j[i].end())
		{
			*(float*)value = j[i][valueName.c_str()];	
		}
	}
}

void DragFieldInt::OnEditor()
{
	ImGui::DragInt((valueName + "##" + className).c_str(), (int*)value);
}

void DragFieldInt::OnSerialize(json& j)
{
	json _j;
	_j[valueName.c_str()] = *(int*)value;
	j.push_back(_j);
}

void DragFieldInt::OnDeserialize(json& j)
{
	for (int i = 0; i < j.size(); i++)
	{
		if (j[i].find(valueName) != j[i].end())
		{
			*(int*)value = j[i][valueName.c_str()];
		}
	}
}

void CheckBoxField::OnEditor()
{
	ImGui::Checkbox((valueName + "##" + className).c_str(), (bool*)value);
}

void CheckBoxField::OnSerialize(json& j)
{
	json _j;
	_j[valueName.c_str()] = *(bool*)value;
	j.push_back(_j);
}

void CheckBoxField::OnDeserialize(json& j)
{
	for (int i = 0; i < j.size(); i++)
	{
		if (j[i].find(valueName) != j[i].end())
		{
			*(bool*)value = j[i][valueName.c_str()];
		}
	}
}

void InputBoxField::OnEditor()
{
	ImGui::InputText((valueName + "##" + className).c_str(), (std::string*)value);
}

void InputBoxField::OnSerialize(json& j)
{
	json _j;
	_j[valueName.c_str()] = *(std::string*)value;
	j.push_back(_j);
}

void InputBoxField::OnDeserialize(json& j)
{
	for (int i = 0; i < j.size(); i++)
	{
		if (j[i].find(valueName) != j[i].end())
		{
			*(std::string*)value = j[i][valueName.c_str()];
		}
	}
}

void DragBoxGameObject::OnEditor()
{
	API::API_GameObject* go = (API::API_GameObject*)value;
	
	std::string buttonName = "X##" + std::to_string(UID);
	if (ImGui::Button(buttonName.c_str()))
	{
		go->SetGameObject(nullptr);
	}
	ImGui::SameLine();

	ImGui::TextWrapped((valueName + ": ").c_str()); ImGui::SameLine();

	if (go->_gameObject == nullptr)
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "NULL (Drag a GameObject here)");
	else
		ImGui::TextColored(ImVec4(1, 1, 0, 1), go->_gameObject->name.c_str());

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
		{
			const uint* drop = (uint*)payload->Data;

			GameObject* droppedGO = ModuleLayers::S_GetGameObject(*drop);

			if (droppedGO != nullptr)
				go->SetGameObject(droppedGO);
		}
		ImGui::EndDragDropTarget();
	}
}

void DragBoxGameObject::OnSerialize(json& j)
{
	json _j;
	
	API::API_GameObject* go = (API::API_GameObject*)value;

	if (go->_gameObject != nullptr)
	{
		_j[valueName.c_str()] = go->_gameObject->GetID();
		j.push_back(_j);
	}
	
}

void DragBoxGameObject::OnDeserialize(json& j)
{
	for (int i = 0; i < j.size(); i++)
	{
		if (j[i].find(valueName) != j[i].end())
		{
			uint id = j[i][valueName.c_str()];
			GameObject* gameObject = ModuleLayers::S_GetGameObject(id);

			if (gameObject != nullptr)
			{
				API::API_GameObject* go = (API::API_GameObject*)value;
				go->SetGameObject(gameObject);
			}
		}
	}
}

void DragBoxTransform::OnEditor()
{
	API::API_Transform* transform = (API::API_Transform*)value;

	std::string buttonName = "X##" + std::to_string(UID);
	if (ImGui::Button(buttonName.c_str()))
	{
		transform->SetComponent(nullptr);
	}
	ImGui::SameLine();

	ImGui::TextWrapped((valueName + ": ").c_str()); ImGui::SameLine();

	if (transform->_transform == nullptr)
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "NULL (Drag a Transform here)");
	else
	{
		std::string gameObjectName(transform->GetGameObject().GetName());
		std::string text = "(" + gameObjectName + ")" + ": TransformComponent";
		ImGui::TextColored(ImVec4(1, 1, 0, 1), text.c_str());
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
		{
			const uint* drop = (uint*)payload->Data;

			GameObject* droppedGO = ModuleLayers::S_GetGameObject(*drop);

			if (droppedGO != nullptr)
				transform->SetComponent(droppedGO->transform);
		}
		ImGui::EndDragDropTarget();
	}
}

void DragBoxTransform::OnSerialize(json& j)
{
	json _j;

	API::API_Transform* transform = (API::API_Transform*)value;

	if (transform->_transform != nullptr)
	{
		_j[valueName.c_str()] = transform->_transform->GetGameObject()->GetID();
		j.push_back(_j);
	}
}

void DragBoxTransform::OnDeserialize(json& j)
{
	for (int i = 0; i < j.size(); i++)
	{
		if (j[i].find(valueName) != j[i].end())
		{
			uint id = j[i][valueName.c_str()];
			GameObject* gameObject = ModuleLayers::S_GetGameObject(id);

			if (gameObject != nullptr)
			{
				API::API_Transform* transform = (API::API_Transform*)value;
				transform->SetComponent(gameObject->transform);
			}
		}
	}
}

void DragBoxMeshRenderer::OnEditor()
{
	API::API_MeshRenderer* mesh = (API::API_MeshRenderer*)value;

	std::string buttonName = "X##" + std::to_string(UID);
	if (ImGui::Button(buttonName.c_str()))
	{
		mesh->SetComponent(nullptr);
	}
	ImGui::SameLine();

	ImGui::TextWrapped((valueName + ": ").c_str()); ImGui::SameLine();

	if (mesh->_meshRenderer == nullptr)
	{
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "NULL (Drag a MeshRenderer here)");
	}
	else
	{
		std::string gameObjectName(mesh->GetGameObject().GetName());
		std::string text = "(" + gameObjectName + ")" + ": MeshRenderer";
		ImGui::TextColored(ImVec4(1, 1, 0, 1), text.c_str());
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
		{
			const uint* drop = (uint*)payload->Data;

			GameObject* droppedGO = ModuleLayers::S_GetGameObject(*drop);
			MeshRenderComponent* component = nullptr;

			if (droppedGO != nullptr)
				component = droppedGO->GetComponent<MeshRenderComponent>();
			
			mesh->SetComponent(component);
		}
		ImGui::EndDragDropTarget();
	}
}

void DragBoxMeshRenderer::OnSerialize(json& j)
{
	json _j;

	API::API_MeshRenderer* mesh = (API::API_MeshRenderer*)value;

	if (mesh->_meshRenderer != nullptr)
	{
		_j[valueName.c_str()] = mesh->_meshRenderer->GetGameObject()->GetID();
		j.push_back(_j);
	}
}

void DragBoxMeshRenderer::OnDeserialize(json& j)
{
	for (int i = 0; i < j.size(); i++)
	{
		if (j[i].find(valueName) != j[i].end())
		{
			uint id = j[i][valueName.c_str()];
			GameObject* gameObject = ModuleLayers::S_GetGameObject(id);
			MeshRenderComponent* component = nullptr;
			if (gameObject != nullptr)
				component = gameObject->GetComponent<MeshRenderComponent>();
			if (component != nullptr)
			{
				API::API_MeshRenderer* mesh = (API::API_MeshRenderer*)value;
				mesh->SetComponent(component);
			}
		}
	}
}

ScriptInspectorField::ScriptInspectorField()
{
	UID = HelloUUID::GenerateUUID();
}

void DragBoxCamera::OnEditor()
{
	API::API_Camera* camera = (API::API_Camera*)value;

	std::string buttonName = "X##" + std::to_string(UID);
	if (ImGui::Button(buttonName.c_str()))
	{
		camera->SetComponent(nullptr);
	}
	ImGui::SameLine();

	ImGui::TextWrapped((valueName + ": ").c_str()); ImGui::SameLine();

	if (camera->_camera == nullptr)
	{
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "NULL (Drag a CameraComponent here)");
	}
	else
	{
		std::string gameObjectName(camera->GetGameObject().GetName());
		std::string text = "(" + gameObjectName + ")" + ": CameraComponent";
		ImGui::TextColored(ImVec4(1, 1, 0, 1), text.c_str());
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
		{
			const uint* drop = (uint*)payload->Data;

			GameObject* droppedGO = ModuleLayers::S_GetGameObject(*drop);
			CameraComponent* component = nullptr;

			if (droppedGO != nullptr)
				component = droppedGO->GetComponent<CameraComponent>();

			camera->SetComponent(component);
		}
		ImGui::EndDragDropTarget();
	}
}

void DragBoxCamera::OnSerialize(json& j)
{
	json _j;

	API::API_Camera* camera = (API::API_Camera*)value;

	if (camera->_camera != nullptr)
	{
		_j[valueName.c_str()] = camera->_camera->GetGameObject()->GetID();
		j.push_back(_j);
	}
}

void DragBoxCamera::OnDeserialize(json& j)
{
	for (int i = 0; i < j.size(); i++)
	{
		if (j[i].find(valueName) != j[i].end())
		{
			uint id = j[i][valueName.c_str()];
			GameObject* gameObject = ModuleLayers::S_GetGameObject(id);
			CameraComponent* component = nullptr;
			if (gameObject != nullptr)
				component = gameObject->GetComponent<CameraComponent>();
			if (component != nullptr)
			{
				API::API_Camera* camera = (API::API_Camera*)value;
				camera->SetComponent(component);
			}
		}
	}
}

void DragBoxRigidBody::OnEditor()
{
	API::API_RigidBody* rigidBody = (API::API_RigidBody*)value;

	std::string buttonName = "X##" + std::to_string(UID);
	if (ImGui::Button(buttonName.c_str()))
	{
		rigidBody->SetComponent(nullptr);
	}
	ImGui::SameLine();

	ImGui::TextWrapped((valueName + ": ").c_str()); ImGui::SameLine();

	if (rigidBody->_rigidBody == nullptr)
	{
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "NULL (Drag a RigidBody here)");
	}
	else
	{
		std::string gameObjectName(rigidBody->GetGameObject().GetName());
		std::string text = "(" + gameObjectName + ")" + ": RigidBody";
		ImGui::TextColored(ImVec4(1, 1, 0, 1), text.c_str());
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
		{
			const uint* drop = (uint*)payload->Data;

			GameObject* droppedGO = ModuleLayers::S_GetGameObject(*drop);
			PhysicsComponent* component = nullptr;

			if (droppedGO != nullptr)
				component = droppedGO->GetComponent<PhysicsComponent>();

			rigidBody->SetComponent(component);
		}
		ImGui::EndDragDropTarget();
	}
}

void DragBoxRigidBody::OnSerialize(json& j)
{
	json _j;

	API::API_RigidBody* rigidBody = (API::API_RigidBody*)value;

	if (rigidBody->_rigidBody != nullptr)
	{
		_j[valueName.c_str()] = rigidBody->_rigidBody->GetGameObject()->GetID();
		j.push_back(_j);
	}
}

void DragBoxRigidBody::OnDeserialize(json& j)
{
	for (int i = 0; i < j.size(); i++)
	{
		if (j[i].find(valueName) != j[i].end())
		{
			uint id = j[i][valueName.c_str()];
			GameObject* gameObject = ModuleLayers::S_GetGameObject(id);
			PhysicsComponent* component = nullptr;
			if (gameObject != nullptr)
				component = gameObject->GetComponent<PhysicsComponent>();
			if (component != nullptr)
			{
				API::API_RigidBody* rigidBody = (API::API_RigidBody*)value;
				rigidBody->SetComponent(component);
			}
		}
	}
}

void DragBoxAnimationPlayer::OnEditor()
{
	API::API_AnimationPlayer* animation = (API::API_AnimationPlayer*)value;

	std::string buttonName = "X##" + std::to_string(UID);
	if (ImGui::Button(buttonName.c_str()))
	{
		animation->SetComponent(nullptr);
	}
	ImGui::SameLine();

	ImGui::TextWrapped((valueName + ": ").c_str()); ImGui::SameLine();

	if (animation->_animation == nullptr)
	{
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "NULL (Drag an AnimationPlayer here)");
	}
	else
	{
		std::string gameObjectName(animation->GetGameObject().GetName());
		std::string text = "(" + gameObjectName + ")" + ": AnimationPlayer";
		ImGui::TextColored(ImVec4(1, 1, 0, 1), text.c_str());
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
		{
			const uint* drop = (uint*)payload->Data;

			GameObject* droppedGO = ModuleLayers::S_GetGameObject(*drop);
			AnimationComponent* component = nullptr;

			if (droppedGO != nullptr)
				component = droppedGO->GetComponent<AnimationComponent>();

			animation->SetComponent(component);
		}
		ImGui::EndDragDropTarget();
	}
}

void DragBoxAnimationPlayer::OnSerialize(json& j)
{
	json _j;

	API::API_AnimationPlayer* animation = (API::API_AnimationPlayer*)value;

	if (animation->_animation != nullptr)
	{
		_j[valueName.c_str()] = animation->_animation->GetGameObject()->GetID();
		j.push_back(_j);
	}
}

void DragBoxAnimationPlayer::OnDeserialize(json& j)
{
	for (int i = 0; i < j.size(); i++)
	{
		if (j[i].find(valueName) != j[i].end())
		{
			uint id = j[i][valueName.c_str()];
			GameObject* gameObject = ModuleLayers::S_GetGameObject(id);
			AnimationComponent* component = nullptr;
			if (gameObject != nullptr)
				component = gameObject->GetComponent<AnimationComponent>();
			if (component != nullptr)
			{
				API::API_AnimationPlayer* animation = (API::API_AnimationPlayer*)value;
				animation->SetComponent(component);
			}
		}
	}
}

void DragBoxAnimationResource::OnEditor()
{
	uint* animationUID = (uint*)value;

	ImGui::TextWrapped((valueName + ": ").c_str()); ImGui::SameLine();

	if (*animationUID == 0)
	{
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "NULL (Drag an AnimationResource here)");
	}
	else
	{
		ResourceAnimation* animRes = (ResourceAnimation*)ModuleResourceManager::resources[*animationUID];
		std::string gameObjectName(animRes->debugName);
		std::string text = "(" + gameObjectName + ")" + ": AnimationResource";
		ImGui::TextColored(ImVec4(1, 1, 0, 1), text.c_str());
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Animation"))
		{
			const uint* drop = (uint*)payload->Data;

			ResourceAnimation* animRes = (ResourceAnimation*)ModuleResourceManager::S_LoadResource(*drop);

			if (animRes != nullptr)
				*animationUID = animRes->UID;
		}
		ImGui::EndDragDropTarget();
	}
}

void DragBoxAnimationResource::OnSerialize(json& j)
{
	json _j;

	uint* animationUID = (uint*)value;

	if (*animationUID != 0)
	{
		_j[valueName.c_str()] = *animationUID;
		j.push_back(_j);
	}
}

void DragBoxAnimationResource::OnDeserialize(json& j)
{
	for (int i = 0; i < j.size(); i++)
	{
		if (j[i].find(valueName) != j[i].end())
		{
			*(uint*)value = j[i][valueName.c_str()];
		}
	}
}

void DragBoxUIButton::OnEditor()
{
	API::API_UIButton* buttonui = (API::API_UIButton*)value;

	std::string buttonName = "X##" + std::to_string(UID);
	if (ImGui::Button(buttonName.c_str()))
	{
		buttonui->SetComponent(nullptr);
	}
	ImGui::SameLine();

	ImGui::TextWrapped((valueName + ": ").c_str()); ImGui::SameLine();

	if (buttonui->_UIButton == nullptr)
	{
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "NULL (Drag an UI Button Here)");
	}
	else
	{
		std::string gameObjectName(buttonui->GetGameObject().GetName());
		std::string text = "(" + gameObjectName + ")" + ": UI Button";
		ImGui::TextColored(ImVec4(1, 1, 0, 1), text.c_str());
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
		{
			const uint* drop = (uint*)payload->Data;

			GameObject* droppedGO = ModuleLayers::S_GetGameObject(*drop);
			ComponentUIButton* component = nullptr;

			if (droppedGO != nullptr)
				component = droppedGO->GetComponent<ComponentUIButton>();

			buttonui->SetComponent(component);
		}
		ImGui::EndDragDropTarget();
	}
}

void DragBoxUIButton::OnSerialize(json& j)
{
	json _j;

	API::API_UIButton* buttonui = (API::API_UIButton*)value;

	if (buttonui->_UIButton != nullptr)
	{
		_j[valueName.c_str()] = buttonui->_UIButton->GetGameObject()->GetID();
		j.push_back(_j);
	}
}

void DragBoxUIButton::OnDeserialize(json& j)
{
	for (int i = 0; i < j.size(); i++)
	{
		if (j[i].find(valueName) != j[i].end())
		{
			uint id = j[i][valueName.c_str()];
			GameObject* gameObject = ModuleLayers::S_GetGameObject(id);
			ComponentUIButton* component = nullptr;
			if (gameObject != nullptr)
				component = gameObject->GetComponent<ComponentUIButton>();
			if (component != nullptr)
			{
				API::API_UIButton* buttonui = (API::API_UIButton*)value;
				buttonui->SetComponent(component);
			}
		}
	}
}

void DragBoxUIImage::OnEditor()
{
	API::API_UIImage* imageui = (API::API_UIImage*)value;

	std::string buttonName = "X##" + std::to_string(UID);
	if (ImGui::Button(buttonName.c_str()))
	{
		imageui->SetComponent(nullptr);
	}
	ImGui::SameLine();

	ImGui::TextWrapped((valueName + ": ").c_str()); ImGui::SameLine();

	if (imageui->_UIImage == nullptr)
	{
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "NULL (Drag an UI Button Here)");
	}
	else
	{
		std::string gameObjectName(imageui->GetGameObject().GetName());
		std::string text = "(" + gameObjectName + ")" + ": UI Button";
		ImGui::TextColored(ImVec4(1, 1, 0, 1), text.c_str());
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
		{
			const uint* drop = (uint*)payload->Data;

			GameObject* droppedGO = ModuleLayers::S_GetGameObject(*drop);
			ComponentUIImage* component = nullptr;

			if (droppedGO != nullptr)
				component = droppedGO->GetComponent<ComponentUIImage>();

			imageui->SetComponent(component);
		}
		ImGui::EndDragDropTarget();
	}
}

void DragBoxUIImage::OnSerialize(json& j)
{
	json _j;

	API::API_UIImage* imageui = (API::API_UIImage*)value;

	if (imageui->_UIImage != nullptr)
	{
		_j[valueName.c_str()] = imageui->_UIImage->GetGameObject()->GetID();
		j.push_back(_j);
	}
}

void DragBoxUIImage::OnDeserialize(json& j)
{
	for (int i = 0; i < j.size(); i++)
	{
		if (j[i].find(valueName) != j[i].end())
		{
			uint id = j[i][valueName.c_str()];
			GameObject* gameObject = ModuleLayers::S_GetGameObject(id);
			ComponentUIImage* component = nullptr;
			if (gameObject != nullptr)
				component = gameObject->GetComponent<ComponentUIImage>();
			if (component != nullptr)
			{
				API::API_UIImage* buttonui = (API::API_UIImage*)value;
				buttonui->SetComponent(component);
			}
		}
	}
}
