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

#ifdef STANDALONE
	void OnEditor() override;

	void MarkAsDead() override;
	void MarkAsAlive() override;

#endif // STANDALONE


	int GetResourceUID();

private:
	void MaterialDragNDrop();
	void ShaderSelectCombo();

private:
	ResourceMaterial* _resource = nullptr;
private:

	uint _resourceUID = 0;
};

