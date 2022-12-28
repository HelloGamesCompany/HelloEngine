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

	// Add script variables to inspector methods:
	void AddDragFloat(std::string name, float* value) override;

private:
	void ImGuiDragScript();
	void DestroyInspectorFields();
private:
	uint scriptUID = 0;
	ResourceScript* scriptResource = nullptr;

	friend class LayerGame;
};

