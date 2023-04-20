#pragma once
#include "LightComponent.h"

struct DirectionalLight : public Light
{
	DirectionalLight() : Light(Component::Type::DIRECTIONAL_LIGHT) {}

	float3 direction;
};

class DirectionalLightComponent : public LightComponent
{
public:
	DirectionalLightComponent(GameObject* gameObject);
	~DirectionalLightComponent();

	void SerializationUnique(json& j) override;
	void DeSerializationUnique(json& j) override;

	void OnTransformCallback(float4x4 worldMatrix) override;

#ifdef STANDALONE
	void OnEditorUnique() override;
#endif

public:
	DirectionalLight data;

public:
	DirectionalLight& GetData() override { return data; }
	void SetData(DirectionalLight& data) { this->data = data; }
};

