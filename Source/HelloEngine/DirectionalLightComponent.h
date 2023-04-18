#pragma once
#include "LightComponent.h"

struct DirectionalLight;

class DirectionalLightComponent : public LightComponent
{
public:
	DirectionalLightComponent(GameObject* gameObject);
	~DirectionalLightComponent();

	void Serialization(json& j) override;
	void DeSerialization(json& j) override;

#ifdef STANDALONE
	void OnEditorUnique() override;
#endif

protected:
	struct DirectionalLight : Light
	{
		DirectionalLight() : Light(Component::Type::DIRECTIONAL_LIGHT) {}

		float3 direction;
	} data;

public:
	DirectionalLight& GetData() override { return data; }
	void SetData(DirectionalLight& data) { this->data = data; }
};

