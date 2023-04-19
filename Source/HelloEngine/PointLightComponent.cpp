#include "Headers.h"
#include "PointLightComponent.h"

#include "ModuleRenderer3D.h"
#include "RenderManager.h"

PointLightComponent::PointLightComponent(GameObject* gameObject) : LightComponent(gameObject)
{
	_type = Type::POINT_LIGHT;
	_name = "Point Light";

	data = PointLight();

	_lightID = Application::Instance()->renderer3D->renderManager.AddLight(data);
}

PointLightComponent::~PointLightComponent()
{

}

void PointLightComponent::OnTransformCallback(float4x4 worldMatrix)
{
	data.position = worldMatrix.TranslatePart();

	//update position
	Application::Instance()->renderer3D->renderManager._lightMap[_lightID] = data;
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
}

#ifdef STANDALONE
void PointLightComponent::OnEditorUnique()
{
	ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Attenuation");
	ImGui::DragFloat("Constant", &data.constant);
	ImGui::DragFloat("Linear", &data.linear);
	ImGui::DragFloat("Exponential", &data.exp);
}
#endif