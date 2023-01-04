#pragma once
#include "Component.h"
#include "ModuleResourceManager.h"
#include "ScriptToInspectorInterface.h"

class ScriptComponent : public Component, public ScriptToInspectorInterface
{
public:
	ScriptComponent(GameObject* go);
	~ScriptComponent();

	void OnEditor() override;

	void OnEnable() override;
	void OnDisable() override;

	void Serialization(json& j) override;
	void DeSerialization(json& j) override;

	void AddScript(std::string scriptName);

	// Add script variables to inspector methods:
	void AddDragFloat(const std::string& name, float* value) override;
	void AddDragInt(const std::string& name, int* value) override;
	void AddCheckBox(const std::string& name, bool* value) override;
	void AddInputBox(const std::string& name, std::string* value) override;
	void AddDragBoxGameObject(const std::string& name, API::API_GameObject* value) override;
	void AddDragBoxTransform(const std::string& name, API::API_Transform* value) override;
private:
	void ImGuiDragScript();
	void DestroyInspectorFields();
	void SaveInspectorFields(json* j = nullptr);
	void LoadInspectorFields(json* j = nullptr);
private:
	uint scriptUID = 0;
	ResourceScript* scriptResource = nullptr;
	json inspectorFieldsJSON;
	std::string addedScript = "None"; // Name of a class that has been added using AddScript() method of API_Gameobject. Not attached to a Resource.
									
	friend class LayerGame;
};

