#include "Headers.h"
#include "ScriptToInspectorInterface.h"

void DragFieldFloat::OnEditor()
{
	ImGui::DragFloat(valueName.c_str(), (float*)value);
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
	ImGui::DragInt(valueName.c_str(), (int*)value);
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
	ImGui::Checkbox(valueName.c_str(), (bool*)value);
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
	ImGui::InputText(valueName.c_str(), (std::string*)value);
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
