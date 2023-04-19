#pragma once
#include "LightComponent.h"

class PointLightComponent : public LightComponent
{
public:
	PointLightComponent(GameObject* gameObject);
	~PointLightComponent();

	void SerializationUnique(json& j) override;
	void DeSerializationUnique(json& j) override;

	void OnTransformCallback(float4x4 worldMatrix) override;

#ifdef STANDALONE
	void OnEditorUnique() override;
#endif

protected:
	struct PointLight : Light
	{
		PointLight() : Light(Component::Type::POINT_LIGHT) {}

		float constant = 1.0f;
		float linear = 1.0f;
		float exp = 1.0f;

		float3 position;

	} data;

public:
	PointLight& GetData() override { return data; }
	void SetData(PointLight& data) { this->data = data; }
};
	

