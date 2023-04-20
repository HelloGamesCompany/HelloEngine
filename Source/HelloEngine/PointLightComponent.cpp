#include "Headers.h"
#include "PointLightComponent.h"

#include "Lighting.h"

PointLightComponent::PointLightComponent(GameObject* gameObject) : LightComponent(gameObject)
{
	_type = Type::POINT_LIGHT;
	_name = "Point Light";

	data = PointLight();
	data = this->data;

	_lightID = Lighting::AddPointLight(this->data);
}

PointLightComponent::~PointLightComponent()
{

}

void PointLightComponent::OnTransformCallback(float4x4 worldMatrix)
{
	data.position = worldMatrix.TranslatePart();

	//update position
	UpdateData(this->data);
	Lighting::GetLightMap().pointLight[_lightID] = this->data;
}

void PointLightComponent::SerializationUnique(json& j)
{
	j["Constant"] = data.constant;
	j["Linear"] = data.linear;
	j["Exp"] = data.exp;
}

void PointLightComponent::DeSerializationUnique(json& j)
{
	data.constant = j["Constant"];
	data.linear = j["Linear"];
	data.exp = j["Exp"];

	UpdateData(this->data);
	Lighting::GetLightMap().pointLight[_lightID] = this->data;
}

#ifdef STANDALONE
void PointLightComponent::OnEditorUnique()
{
	UpdateData(this->data);
	ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Attenuation");
	ImGui::DragFloat("Constant", &data.constant);
	ImGui::DragFloat("Linear", &data.linear);
	ImGui::DragFloat("Exponential", &data.exp);

	Lighting::GetLightMap().pointLight[_lightID] = this->data;
}
#endif