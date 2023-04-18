#include "Headers.h"
#include "DirectionalLightComponent.h"

#include "ModuleRenderer3D.h"
#include "RenderManager.h"

DirectionalLightComponent::DirectionalLightComponent(GameObject* gameObject) : LightComponent(gameObject)
{
	_type = Type::DIRECTIONAL_LIGHT;
	_name = "Directional Light";

	data = DirectionalLight();

	Application::Instance()->renderer3D->renderManager.AddLight(data);
}

DirectionalLightComponent::~DirectionalLightComponent()
{

}

void DirectionalLightComponent::Serialization(json& j)
{

}

void DirectionalLightComponent::DeSerialization(json& j)
{

}

#ifdef STANDALONE
void DirectionalLightComponent::OnEditorUnique()
{
	ImGui::DragFloat3("Direction", &data.direction.At(0), 0.05f, -1.0f, 1.0f);
}
#endif