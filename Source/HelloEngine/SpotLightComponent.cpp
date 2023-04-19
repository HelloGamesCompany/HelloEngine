#include "Headers.h"
#include "SpotLightComponent.h"

#include "ModuleRenderer3D.h"
#include "RenderManager.h"

SpotLightComponent::SpotLightComponent(GameObject* gameObject) : LightComponent(gameObject)
{
	_type = Type::SPOT_LIGHT;
	_name = "Spot Light";

	data = SpotLight();

	_lightID = Application::Instance()->renderer3D->renderManager.AddLight(data);
}

SpotLightComponent::~SpotLightComponent()
{

}

void SpotLightComponent::OnTransformCallback(float4x4 worldMatrix)
{
	data.position = worldMatrix.TranslatePart();
	data.direction = worldMatrix.ToEulerXYZ();

	Application::Instance()->renderer3D->renderManager._lightMap[_lightID] = data;
}

void SpotLightComponent::SerializationUnique(json& j)
{
	j["Cutoff"] = data.cutoff;

	j["Constant"] = data.constant;
	j["Linear"] = data.linear;
	j["Exp"] = data.exp;
}

void SpotLightComponent::DeSerializationUnique(json& j)
{
	data.cutoff = j["Cutoff"];

	data.constant = j["Constant"];
	data.linear = j["Linear"];
	data.exp = j["Exp"];
}

#ifdef STANDALONE
void SpotLightComponent::OnEditorUnique()
{
	ImGui::DragFloat("Cutoff", &data.cutoff);

	ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Attenuation");
	ImGui::DragFloat("Constant", &data.constant);
	ImGui::DragFloat("Linear", &data.linear);
	ImGui::DragFloat("Exponential", &data.exp);
}
#endif