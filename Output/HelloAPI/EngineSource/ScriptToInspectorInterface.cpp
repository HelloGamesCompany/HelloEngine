#include "Headers.h"
#include "ScriptToInspectorInterface.h"
#include "ModuleLayers.h"
#include "API/API_Transform.h"
#include "API/API_MeshRenderer.h"
#include "API/API_Camera.h"
#include "API/API_RigidBody.h"
#include "API/API_AnimationPlayer.h"
#include "API/API_ParticleSystem.h"
#include "API/API_Material.h"

#include "PhysicsComponent.h"
#include "MeshRenderComponent.h"
#include "CameraComponent.h"
#include "AnimationComponent.h"
#include "ParticleSystemComponent.h"
#include "MaterialComponent.h"

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

void DragBoxMeshResource::OnEditor()
{
	uint* meshUID = (uint*)value;

	ImGui::TextWrapped((valueName + ": ").c_str()); ImGui::SameLine();

	if (*meshUID == 0)
	{
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "NULL (Drag a Mesh Resource here)");
	}
	else
	{
		ResourceMesh* meshRes = (ResourceMesh*)ModuleResourceManager::resources[*meshUID];
		if (meshRes != nullptr)
		{
			std::string gameObjectName(meshRes->debugName);
			std::string text = "(" + gameObjectName + ")" + ": Mesh Resource";
			ImGui::TextColored(ImVec4(1, 1, 0, 1), text.c_str());
		}
		else
		{
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "Reference Lost! Drag the Mesh again here.");
		}
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Mesh"))
		{
			const uint* drop = (uint*)payload->Data;

			ResourceMesh* meshRes = (ResourceMesh*)ModuleResourceManager::S_LoadResource(*drop);

			if (meshRes != nullptr)
				*meshUID = meshRes->UID;
		}
		ImGui::EndDragDropTarget();
	}
}

void DragBoxMeshResource::OnSerialize(json& j)
{
	json _j;

	uint* meshUID = (uint*)value;

	if (*meshUID != 0)
	{
		ResourceMesh* mesh = (ResourceMesh*)ModuleResourceManager::resources[*meshUID];
		_j[(valueName + "ModelUID").c_str()] = mesh->modelUID;
		_j[(valueName + "IndexInsideModel").c_str()] = mesh->indexInsideModel;
		j.push_back(_j);
	}
}

void DragBoxMeshResource::OnDeserialize(json& j)
{
	for (int i = 0; i < j.size(); i++)
	{
		if (j[i].find((valueName + "ModelUID").c_str()) != j[i].end())
		{
			uint modelUID = j[i][(valueName + "ModelUID").c_str()];
			ResourceModel* model = (ResourceModel*)ModuleResourceManager::resources[modelUID];

			uint indexInModel = j[i][(valueName + "IndexInsideModel").c_str()];

			if (indexInModel < model->modelMeshes.size())
			{
				ResourceMesh* resourceMesh = model->modelMeshes[indexInModel];
				*(uint*)value = resourceMesh->UID;
			}
		}
	}
}

void DragBoxTextureResource::OnEditor()
{
	uint* textureUID = (uint*)value;

	ImGui::TextWrapped((valueName + ": ").c_str()); ImGui::SameLine();

	if (*textureUID == 0)
	{
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "NULL (Drag a Texture Resource here)");
	}
	else
	{
		ResourceTexture* textureRes = (ResourceTexture*)ModuleResourceManager::resources[*textureUID];
		std::string gameObjectName(textureRes->debugName);
		std::string text = "(" + gameObjectName + ")" + ": Texture Resource";
		ImGui::TextColored(ImVec4(1, 1, 0, 1), text.c_str());
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Texture"))
		{
			const uint* drop = (uint*)payload->Data;

			ResourceTexture* textureRes = (ResourceTexture*)ModuleResourceManager::S_LoadResource(*drop);

			if (textureRes != nullptr)
				*textureUID = textureRes->UID;
		}
		ImGui::EndDragDropTarget();
	}
}

void DragBoxTextureResource::OnSerialize(json& j)
{
	json _j;

	uint* textureUID = (uint*)value;

	if (*textureUID != 0)
	{
		_j[valueName.c_str()] = *textureUID;
		j.push_back(_j);
	}
}

void DragBoxTextureResource::OnDeserialize(json& j)
{
	for (int i = 0; i < j.size(); i++)
	{
		if (j[i].find(valueName) != j[i].end())
		{
			*(uint*)value = j[i][valueName.c_str()];
		}
	}
}

void DragBoxMaterialComponent::OnEditor()
{
	API::API_Material* material = (API::API_Material*)value;

	std::string buttonName = "X##" + std::to_string(UID);
	if (ImGui::Button(buttonName.c_str()))
	{
		material->SetComponent(nullptr);
	}
	ImGui::SameLine();

	ImGui::TextWrapped((valueName + ": ").c_str()); ImGui::SameLine();

	if (material->_material == nullptr)
	{
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "NULL (Drag a Material Component here)");
	}
	else
	{
		std::string gameObjectName(material->GetGameObject().GetName());
		std::string text = "(" + gameObjectName + ")" + ": Material Component";
		ImGui::TextColored(ImVec4(1, 1, 0, 1), text.c_str());
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
		{
			const uint* drop = (uint*)payload->Data;

			GameObject* droppedGO = ModuleLayers::S_GetGameObject(*drop);
			MaterialComponent* component = nullptr;

			if (droppedGO != nullptr)
				component = droppedGO->GetComponent<MaterialComponent>();

			material->SetComponent(component);
		}
		ImGui::EndDragDropTarget();
	}
}

void DragBoxMaterialComponent::OnSerialize(json& j)
{
	json _j;

	API::API_Material* material = (API::API_Material*)value;

	if (material->_material != nullptr)
	{
		_j[valueName.c_str()] = material->_material->GetGameObject()->GetID();
		j.push_back(_j);
	}
}

void DragBoxMaterialComponent::OnDeserialize(json& j)
{
	for (int i = 0; i < j.size(); i++)
	{
		if (j[i].find(valueName) != j[i].end())
		{
			uint id = j[i][valueName.c_str()];
			GameObject* gameObject = ModuleLayers::S_GetGameObject(id);
			MaterialComponent* component = nullptr;
			if (gameObject != nullptr)
				component = gameObject->GetComponent<MaterialComponent>();
			if (component != nullptr)
			{
				API::API_Material* material = (API::API_Material*)value;
				material->SetComponent(component);
			}
		}
	}
}

void DragBoxParticleSystem::OnEditor()
{
	API::API_ParticleSystem* particlesystem = (API::API_ParticleSystem*)value;

	std::string buttonName = "X##" + std::to_string(UID);
	if (ImGui::Button(buttonName.c_str()))
	{
		particlesystem->SetComponent(nullptr);
	}
	ImGui::SameLine();

	ImGui::TextWrapped((valueName + ": ").c_str()); ImGui::SameLine();

	if (particlesystem->_particleSystem == nullptr)
	{
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "NULL (Drag a Particle System here)");
	}
	else
	{
		std::string gameObjectName(particlesystem->GetGameObject().GetName());
		std::string text = "(" + gameObjectName + ")" + ": Particle System";
		ImGui::TextColored(ImVec4(1, 1, 0, 1), text.c_str());
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
		{
			const uint* drop = (uint*)payload->Data;

			GameObject* droppedGO = ModuleLayers::S_GetGameObject(*drop);
			ParticleSystemComponent* component = nullptr;

			if (droppedGO != nullptr)
				component = droppedGO->GetComponent<ParticleSystemComponent>();

			particlesystem->SetComponent(component);
		}
		ImGui::EndDragDropTarget();
	}
}

void DragBoxParticleSystem::OnSerialize(json& j)
{
	json _j;

	API::API_ParticleSystem* particlesystem = (API::API_ParticleSystem*)value;

	if (particlesystem->_particleSystem != nullptr)
	{
		_j[valueName.c_str()] = particlesystem->_particleSystem->GetGameObject()->GetID();
		j.push_back(_j);
	}
}

void DragBoxParticleSystem::OnDeserialize(json& j)
{
	for (int i = 0; i < j.size(); i++)
	{
		if (j[i].find(valueName) != j[i].end())
		{
			uint id = j[i][valueName.c_str()];
			GameObject* gameObject = ModuleLayers::S_GetGameObject(id);
			ParticleSystemComponent* component = nullptr;
			if (gameObject != nullptr)
				component = gameObject->GetComponent<ParticleSystemComponent>();
			if (component != nullptr)
			{
				API::API_ParticleSystem* particlesystem = (API::API_ParticleSystem*)value;
				particlesystem->SetComponent(component);
			}
		}
	}
}
