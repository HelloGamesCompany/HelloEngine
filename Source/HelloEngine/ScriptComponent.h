#pragma once
#include "Component.h"
#include "ModuleResourceManager.h"

class ScriptComponent : public Component
{
public:
	ScriptComponent(GameObject* go);
	~ScriptComponent();

	void OnEditor() override;

	void OnEnable() override;
	void OnDisable() override;

	void Serialization(json& j) override;
	void DeSerialization(json& j) override;

private:
	void ImGuiDragScript();
private:
	uint scriptUID = 0;
	ResourceScript* scriptResource = nullptr;

	friend class LayerGame;
};

