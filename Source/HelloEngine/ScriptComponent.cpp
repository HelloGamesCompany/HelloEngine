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
		if (scriptResource == nullptr && addedScript == "None")
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
			ImGui::TextColored(ImVec4(1, 1, 0, 1), scriptResource == nullptr ? addedScript.c_str() : scriptResource->className.c_str());
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

void ScriptComponent::AddScript(std::string scriptName)
{
	addedScript = scriptName;// We need to do this before calling next funciton because the addedScript is needed by the Create function.

	// Create a new script object instance.
	if (!LayerGame::S_CreateBehaviorScriptByName(scriptName, this))
	{
		addedScript = "None"; 
		return;
	}
}

void ScriptComponent::AddDragFloat(const char* name, float* value)
{
	DragFieldFloat* dragField = new DragFieldFloat();
	dragField->valueName = name;
	dragField->value = value;
	dragField->className = scriptResource == nullptr ? addedScript : scriptResource->className;

	inspectorFields.push_back(dragField);
}

void ScriptComponent::AddDragInt(const char* name, int* value)
{
	DragFieldInt* dragField = new DragFieldInt();
	dragField->valueName = name;
	dragField->value = value;
	dragField->className = scriptResource == nullptr ? addedScript : scriptResource->className;

	inspectorFields.push_back(dragField);
}

void ScriptComponent::AddCheckBox(const char* name, bool* value)
{
	CheckBoxField* checkBoxField = new CheckBoxField();
	checkBoxField->valueName = name;
	checkBoxField->value = value;
	checkBoxField->className = scriptResource == nullptr ? addedScript : scriptResource->className;

	inspectorFields.push_back(checkBoxField);
}

void ScriptComponent::AddInputBox(const char* name, std::string* value)
{
	InputBoxField* inputBoxField = new InputBoxField();
	inputBoxField->valueName = name;
	inputBoxField->value = value;
	inputBoxField->className = scriptResource == nullptr ? addedScript : scriptResource->className;

	inspectorFields.push_back(inputBoxField);
}

void ScriptComponent::AddDragBoxGameObject(const char* name, API::API_GameObject* value)
{
	DragBoxGameObject* dragBoxField = new DragBoxGameObject();
	dragBoxField->valueName = name;
	dragBoxField->value = value;
	dragBoxField->className = scriptResource == nullptr ? addedScript : scriptResource->className;

	inspectorFields.push_back(dragBoxField);
}

void ScriptComponent::AddDragBoxTransform(const char* name, API::API_Transform* value)
{
	DragBoxTransform* dragBoxField = new DragBoxTransform();
	dragBoxField->valueName = name;
	dragBoxField->value = value;
	dragBoxField->className = scriptResource == nullptr ? addedScript : scriptResource->className;

	inspectorFields.push_back(dragBoxField);
}

void ScriptComponent::AddDragBoxMeshRenderer(const char* name, API::API_MeshRenderer* value)
{
	DragBoxMeshRenderer* dragBoxField = new DragBoxMeshRenderer();
	dragBoxField->valueName = name;
	dragBoxField->value = value;
	dragBoxField->className = scriptResource == nullptr ? addedScript : scriptResource->className;

	inspectorFields.push_back(dragBoxField);
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
