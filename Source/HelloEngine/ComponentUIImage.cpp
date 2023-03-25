#include "Headers.h"
#include "ComponentUIImage.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "ImWindowGame.h"
#include "LayerEditor.h"

ComponentUIImage::ComponentUIImage(GameObject* gameObject) : ComponentUI(gameObject)
{
	_type = Component::Type::UI_IMAGE;

	_fillImage = 1.f;
	_maxScale = 1.f;
	_gameWindow = (ImWindowGame*)LayerEditor::_imWindows[(uint)ImWindowID::GAME];
}

ComponentUIImage::~ComponentUIImage()
{
}

void ComponentUIImage::InputUpdate()
{

	//if()

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

	ImGui::Text("Fill Image");
	ImGui::SameLine();

	//fill image
	float aux1 = _fillImage;
	if (ImGui::DragFloat("##fill", &_fillImage, 0.001f, 0, _maxScale))
	{
		this->_gameObject->transform->SetScale({ _fillImage, this->_gameObject->transform->GetLocalScale().y, this->_gameObject->transform->GetLocalScale().z });
		this->_gameObject->transform->SetPosition({ this->_gameObject->transform->GetGlobalPosition().x + (_fillImage - aux1),
													this->_gameObject->transform->GetLocalPosition().y, this->_gameObject->transform->GetLocalPosition().z });
	}

	ImGui::InputFloat("Max Scale", &_maxScale);
	

}
#endif // STANDALONE