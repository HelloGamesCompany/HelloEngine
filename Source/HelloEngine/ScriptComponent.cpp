#include "Headers.h"
#include "ScriptComponent.h"
#include "ModuleLayers.h"
#include "LayerGame.h"
#include "LayerEditor.h"

ScriptComponent::ScriptComponent(GameObject* go) : Component(go)
{
	_type = Component::Type::SCRIPT;
	Application::Instance()->layers->game->AddScriptComponent(this);
}

ScriptComponent::~ScriptComponent()
{
	Application::Instance()->layers->game->RemoveScriptComponent(this);
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
		Application::Instance()->layers->game->_behaviorScripts[scriptUID].active = true;
}

void ScriptComponent::OnDisable()
{
	if (scriptUID != 0)
		Application::Instance()->layers->game->_behaviorScripts[scriptUID].active = false;
}

void ScriptComponent::Serialization(json& j)
{
	json _j;

	_j["Type"] = _type;

	_j["Script Resource"] = scriptResource ? scriptResource->UID : 0;

	// Serialize every inspector field
	for (int i = 0; i < inspectorFields.size(); ++i)
	{
		inspectorFields[i]->OnSerialize(_j["Inspector Fields"]);
	}

	j["Components"].push_back(_j);
}

void ScriptComponent::DeSerialization(json& j)
{
	scriptResource = (ResourceScript*)ModuleResourceManager::S_LoadResource(j["Script Resource"]);

	if (scriptResource)
	{
		// Create a new script object instance.
		Application::Instance()->layers->game->CreateBehaviorScript(this);
		// Deserialize every inspector field
		for (int i = 0; i < inspectorFields.size(); ++i)
		{
			inspectorFields[i]->OnDeserialize(j["Inspector Fields"]);
		}
	}
}

void ScriptComponent::AddDragFloat(std::string name, float* value)
{
	DragField* dragField = new DragField();
	dragField->valueName = name;
	dragField->value = value;

	inspectorFields.push_back(dragField);
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
				Application::Instance()->layers->game->DestroyBehaviorScript(this);
				scriptResource->Dereference();
				DestroyInspectorFields();
			}

			scriptResource = (ResourceScript*)Application::Instance()->resource->S_LoadResource(*drop);

			// Create a new script object instance.
			Application::Instance()->layers->game->CreateBehaviorScript(this);

			std::string popUpmessage = "Loaded Script: " + scriptResource->className;
			Application::Instance()->layers->editor->AddPopUpMessage(popUpmessage);

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
