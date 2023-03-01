#include "Headers.h"
#include "ComponentUIImage.h"
#include "GameObject.h"
#include "MaterialComponent.h"

ComponentUIImage::ComponentUIImage(GameObject* gameObject) : ComponentUI(gameObject)
{
	_type = Component::Type::UI_IMAGE;
	_fillImage = 1.f;
}

ComponentUIImage::~ComponentUIImage()
{
}

void ComponentUIImage::InputUpdate()
{

}

void ComponentUIImage::Serialization(json& j)
{
	json _j;

	_j["Type"] = _type;
	_j["MaterialResource"] = _material->GetResourceUID();
	_j["Enabled"] = _isEnabled;
	_j["FillImage"] = _fillImage;
	j["Components"].push_back(_j);
}

void ComponentUIImage::DeSerialization(json& j)
{
	_material->ChangeTexture((ResourceTexture*)ModuleResourceManager::S_LoadResource(j["MaterialResource"]));

	bool enabled = j["Enabled"];
	if (!enabled)
		Disable();

	_fillImage = j["FillImage"];


	_gameObject->transform->ForceUpdate();

}

#ifdef STANDALONE
void ComponentUIImage::OnEditor()
{

	bool created = true;
	if (!ImGui::CollapsingHeader("Image", &created, ImGuiTreeNodeFlags_DefaultOpen)) return;
	if (!created)
	{
		_gameObject->DestroyComponent(this);
		return;
	}

	/*bool auxiliaryBool = _isEnabled;
	if (ImGui::Checkbox("Active##Material", &auxiliaryBool))
		auxiliaryBool ? Enable() : Disable();*/

	ImGui::Text("Fill Image");
	ImGui::SameLine();
	ImGui::DragFloat("##fill", &_fillImage, 0.001f, 0, 1);

}
#endif // STANDALONE