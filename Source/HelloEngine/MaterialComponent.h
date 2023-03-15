#pragma once
#include "Component.h"

class ResourceMaterial;

class MaterialComponent : public Component
{
public:
	MaterialComponent(GameObject* gameObject);
	~MaterialComponent();

	void Serialization(json& j) override;
	void DeSerialization(json& j) override;

	void OnEditor() override;

private:
	void MaterialDragNDrop();
	void ShaderSelectCombo();

private:
	ResourceMaterial* _resource = nullptr;
};

