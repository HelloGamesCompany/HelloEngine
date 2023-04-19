#pragma once
#include "LightComponent.h"

struct SpotLight;

class SpotLightComponent : public LightComponent
{
public:
	SpotLightComponent(GameObject* gameObject);
	~SpotLightComponent();

	void SerializationUnique(json& j) override;
	void DeSerializationUnique(json& j) override;

	void OnTransformCallback(float4x4 worldMatrix) override;

#ifdef STANDALONE
	void OnEditorUnique() override;
#endif

	struct SpotLight : Light
	{
		SpotLight() : Light(Component::Type::SPOT_LIGHT) {}

		float constant = 1.0f;
		float linear = 1.0f;
		float exp = 1.0f;

		float3 position;
		float3 direction;
		float cutoff;
	} data;

	SpotLight& GetData() override { return data; }
	void SetData(SpotLight& data) { this->data = data; }
};

