#include "Headers.h"
#include "ScriptToInspectorInterface.h"

void DragField::OnEditor()
{
	ImGui::DragFloat(valueName.c_str(), (float*)value);
}

void DragField::OnSerialize(json& j)
{
	json _j;
	_j[valueName.c_str()] = *(float*)value;
	j.push_back(_j);
}

void DragField::OnDeserialize(json& j)
{
	for (int i = 0; i < j.size(); i++)
	{
		if (j[i].find(valueName) != j[i].end())
		{
			*(float*)value = j[i][valueName.c_str()];
		}
	}
}
