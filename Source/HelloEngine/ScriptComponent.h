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

	const std::string& GetScriptName();
	HelloBehavior* GetScript();

	// Add script variables to inspector methods:
	void AddDragFloat(const char* name, float* value) override;
	void AddDragInt(const char* name, int* value) override;
	void AddCheckBox(const char* name, bool* value) override;
	void AddInputBox(const char* name, std::string* value) override;
	void AddDragBoxGameObject(const char* name, API::API_GameObject* value) override;
	void AddDragBoxTransform(const char* name, API::API_Transform* value) override;
	void AddDragBoxMeshRenderer(const char* name, API::API_MeshRenderer* value) override;

	uint GetResourceUID();
	void DestroyedResource();

private:
	void ImGuiDragScript();
	void DestroyInspectorFields();
	void SaveInspectorFields(json* j = nullptr);
	void LoadInspectorFields(json* j = nullptr);

	void MarkAsDead() override;
	void MarkAsAlive() override;

private:
	uint scriptUID = 0;
	ResourceScript* scriptResource = nullptr;
	json inspectorFieldsJSON;
	std::string addedScript = "None"; // Name of a class that has been added using AddScript() method of API_Gameobject. Not attached to a Resource.
									
	friend class LayerGame;
};

