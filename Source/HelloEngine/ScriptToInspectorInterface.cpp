#include "Headers.h"
#include "ScriptToInspectorInterface.h"
#include "ModuleLayers.h"
#include "API/API_Transform.h"
#include "API/API_MeshRenderer.h"
#include "MeshRenderComponent.h"

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
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("APIGameObject"))
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
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("APIGameObject"))
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
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("APIGameObject"))
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
