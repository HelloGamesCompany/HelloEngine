#include "Headers.h"
#include "ScriptComponent.h"
#include "ModuleLayers.h"
#include "LayerGame.h"
#include "LayerEditor.h"

ScriptComponent::ScriptComponent(GameObject* go) : Component(go)
{
	_type = Component::Type::SCRIPT;
	LayerGame::S_AddScriptComponent(this);
}

ScriptComponent::~ScriptComponent()
{
	LayerGame::S_RemoveScriptComponent(this);
	if (scriptResource != nullptr)
		scriptResource->Dereference();
	DestroyInspectorFields();
}

void ScriptComponent::OnEditor()
{
	if (ImGui::CollapsingHeader("Script", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (scriptResource == nullptr)
		{
			ImGui::TextColored(ImVec4(1,1,0,1), "Drag a .cpp or an .h file with a correct HelloBehavior children class.");
			ImGuiDragScript();
		}
		else
		{
			bool auxiliaryBool = _isEnabled;
			if (ImGui::Checkbox("Active##Script", &auxiliaryBool))
				auxiliaryBool ? Enable() : Disable();

			ImGui::TextWrapped("Loaded script: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), scriptResource->className.c_str());
			ImGuiDragScript();
			// Show script inspector variables.
			for (int i = 0; i < inspectorFields.size(); ++i)
			{
				inspectorFields[i]->OnEditor();
			}
		}
	}
}

void ScriptComponent::OnEnable()
{
	if (scriptUID != 0)
		LayerGame::_behaviorScripts[scriptUID].active = true;
}

void ScriptComponent::OnDisable()
{
	if (scriptUID != 0)
		LayerGame::_behaviorScripts[scriptUID].active = false;
}

void ScriptComponent::Serialization(json& j)
{
	json _j;

	_j["Type"] = _type;

	_j["Script Resource"] = scriptResource ? scriptResource->UID : 0;

	// Serialize every inspector field
	SaveInspectorFields(&_j);

	/*for (int i = 0; i < inspectorFields.size(); ++i)
	{
		inspectorFields[i]->OnSerialize(_j["Inspector Fields"]);
	}*/

	j["Components"].push_back(_j);
}

void ScriptComponent::DeSerialization(json& j)
{
	scriptResource = (ResourceScript*)ModuleResourceManager::S_LoadResource(j["Script Resource"]);

	if (scriptResource)
	{
		// Create a new script object instance.
		LayerGame::S_CreateBehaviorScript(this);
		// Deserialize every inspector field
		LoadInspectorFields(&j);
	}
}

void ScriptComponent::AddDragFloat(const std::string& name, float* value)
{
	DragFieldFloat* dragField = new DragFieldFloat();
	dragField->valueName = name;
	dragField->value = value;
	dragField->className = scriptResource->className;

	inspectorFields.push_back(dragField);
}

void ScriptComponent::AddDragInt(const std::string& name, int* value)
{
	DragFieldInt* dragField = new DragFieldInt();
	dragField->valueName = name;
	dragField->value = value;
	dragField->className = scriptResource->className;

	inspectorFields.push_back(dragField);
}

void ScriptComponent::AddCheckBox(const std::string& name, bool* value)
{
	CheckBoxField* checkBoxField = new CheckBoxField();
	checkBoxField->valueName = name;
	checkBoxField->value = value;
	checkBoxField->className = scriptResource->className;

	inspectorFields.push_back(checkBoxField);
}

void ScriptComponent::AddInputBox(const std::string& name, std::string* value)
{
	InputBoxField* inputBoxField = new InputBoxField();
	inputBoxField->valueName = name;
	inputBoxField->value = value;
	inputBoxField->className = scriptResource->className;

	inspectorFields.push_back(inputBoxField);
}

void ScriptComponent::ImGuiDragScript()
{
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Script"))
		{
			const uint* drop = (uint*)payload->Data;

			// If we had another script attached, destroy that script instance first
			if (scriptUID != 0)
			{
				LayerGame::S_DestroyBehaviorScript(this);
				scriptResource->Dereference();
				DestroyInspectorFields();
			}

			scriptResource = (ResourceScript*)ModuleResourceManager::S_LoadResource(*drop);

			// Create a new script object instance.
			LayerGame::S_CreateBehaviorScript(this);

			std::string popUpmessage = "Loaded Script: " + scriptResource->className;
			LayerEditor::S_AddPopUpMessage(popUpmessage);
		}
		ImGui::EndDragDropTarget();
	}

}

void ScriptComponent::DestroyInspectorFields()
{
	for (int i = 0; i < inspectorFields.size(); ++i)
	{
		RELEASE(inspectorFields[i]);
	}
	inspectorFields.clear();
}

void ScriptComponent::SaveInspectorFields(json* j)
{
	// Serialize every inspector field
	for (int i = 0; i < inspectorFields.size(); ++i)
	{
		inspectorFields[i]->OnSerialize(inspectorFieldsJSON["Inspector Fields"]);
	}

	if (j != nullptr)
	{
		json& _j = *j;
		_j["Inspector Fields"] = inspectorFieldsJSON["Inspector Fields"];
	}
}

void ScriptComponent::LoadInspectorFields(json* j)
{
	if (j != nullptr)
	{
		for (int i = 0; i < inspectorFields.size(); ++i)
		{
			json& _j = *j;
			inspectorFields[i]->OnDeserialize(_j["Inspector Fields"]);
		}
	}
	else
	{
		for (int i = 0; i < inspectorFields.size(); ++i)
		{
			inspectorFields[i]->OnDeserialize(inspectorFieldsJSON["Inspector Fields"]);
		}
	}
}
