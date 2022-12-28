#pragma once
#include "Component.h"
#include "ModuleResourceManager.h"

enum class FieldType
{
	DRAG,
	CHECK_BOX,
	INPUT_BOX
};

class ScriptInspectorField
{
public:
	void* value = nullptr;
	std::string valueName = "";
	FieldType type;
	virtual void OnEditor() = 0;
};

class DragField : public ScriptInspectorField
{
	void OnEditor() override;
};

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

	// Add script variables to inspector methods:
	void AddDragFloat(std::string name, float* value);

private:
	void ImGuiDragScript();
	void DestroyInspectorFields();
private:
	std::vector<ScriptInspectorField*> inspectorFields;
	uint scriptUID = 0;
	ResourceScript* scriptResource = nullptr;
	//TEST
	float testFloat = 5.5f;

	friend class LayerGame;
};

