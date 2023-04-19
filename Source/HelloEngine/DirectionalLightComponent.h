#pragma once
#include "LightComponent.h"

struct DirectionalLight;

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
	struct DirectionalLight : Light
	{
		DirectionalLight() : Light(Component::Type::DIRECTIONAL_LIGHT) {}

		float3 direction;
	} data;

public:
	DirectionalLight& GetData() override { return data; }
	void SetData(DirectionalLight& data) { this->data = data; }
};

