#include "Headers.h"
#include "ScriptToInspectorInterface.h"
#include "API/API_GameObject.h"
#include "ModuleLayers.h"

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
	
	if (go->_gameObject == nullptr)
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Drop a GameObject here");
	else
		ImGui::TextColored(ImVec4(1, 1, 0, 1), go->GetName().c_str());

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
}

void DragBoxGameObject::OnDeserialize(json& j)
{
}
