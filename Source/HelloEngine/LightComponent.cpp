#include "Headers.h"
#include "LightComponent.h"
#include "GameObject.h"

LightComponent::LightComponent(GameObject* gameObject) : Component(gameObject)
{
	_type = Type::LIGHT;
	_name = "Light";
}

LightComponent::~LightComponent()
{

}

void LightComponent::Serialization(json& j)
{

}

void LightComponent::DeSerialization(json& j)
{

}

#ifdef STANDALONE
void LightComponent::OnEditor()
{
	bool created = true;

	if (ImGui::CollapsingHeader(_name.c_str(), &created, ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Light Parameters");
		ImGui::Separator();

		//COLOUR
		std::string popName = "##LightColorPicker";
		if (ImGui::ColorButton("Light Colour", (ImVec4&)data.color, 0, { 50, 50 }))
		{
			ImGui::OpenPopup(popName.c_str());
		}
		ImGui::SameLine();
		ImGui::Text("Light Colour");

		if (ImGui::BeginPopup(popName.c_str()))
		{
			ImGui::ColorPicker3("Light Colour", &data.color.At(0), 0 | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);

			ImGui::EndPopup();
		}

		//Ambient intensity
		ImGui::DragFloat("Ambient Intensity", &data.ambientIntensity, 0.25f);

		//Diffuse intensity
		ImGui::DragFloat("Diffuse Intensity", &data.diffuseIntensity, 0.25f);

		//Unique per child
		ImGui::NewLine();
		switch (_type)
		{
			case Type::DIRECTIONAL_LIGHT: ImGui::Text("Directional Parameters"); break;
			case Type::POINT_LIGHT: ImGui::Text("Point Parameters"); break;
			case Type::SPOT_LIGHT: ImGui::Text("Spot Parameters"); break;
		}
		ImGui::Separator();
		OnEditorUnique();

		if (GetData().GetType() == Component::Type::DIRECTIONAL_LIGHT)
		{
			ImGui::Text("Im a Directional Light :D!");
		}

		ImGui::NewLine();
	}

	if (!created)
		this->_gameObject->DestroyComponent(this);
}

void LightComponent::OnEditorUnique()
{

}

void LightComponent::MarkAsDead()
{

}

void LightComponent::MarkAsAlive()
{

}
#endif
