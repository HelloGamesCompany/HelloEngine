#include "Headers.h"
#include "DirectionalLightComponent.h"

#include "Lighting.h"

DirectionalLightComponent::DirectionalLightComponent(GameObject* gameObject) : LightComponent(gameObject)
{
	_type = Type::DIRECTIONAL_LIGHT;
	_name = "Directional Light";

	data = DirectionalLight();

	Lighting::SetDirectionalLight(data);
}

DirectionalLightComponent::~DirectionalLightComponent()
{

}

void DirectionalLightComponent::OnTransformCallback(float4x4 worldMatrix)
{
	data.direction = worldMatrix.ToEulerXYZ();

	Lighting::SetDirectionalLight(data);
}

void DirectionalLightComponent::SerializationUnique(json& j)
{
	//j["Direction"] = { data.direction.x, data.direction.y, data.direction.z };
}

void DirectionalLightComponent::DeSerializationUnique(json& j)
{
	/*std::vector<float> temp = j["Direction"];
	data.direction = { temp[0], temp[1], temp[2] };*/
}

#ifdef STANDALONE
void DirectionalLightComponent::OnEditorUnique()
{
	ImGui::DragFloat3("Direction", &data.direction.At(0), 0.05f, -1.0f, 1.0f);
}
#endif