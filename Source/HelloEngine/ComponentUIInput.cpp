#include "Headers.h"
#include "ComponentUIInput.h"
#include "GameObject.h"
#include "MaterialComponent.h"
#include "ImWindowGame.h"
#include "LayerEditor.h"

ComponentUIInput::ComponentUIInput(GameObject* gameObject) : ComponentUI(gameObject)
{
	_type = Component::Type::UI_INPUT;

	_meshRenderer->Disable();

	_gameWindow = (ImWindowGame*)LayerEditor::_imWindows[(uint)ImWindowID::GAME];
}

ComponentUIInput::~ComponentUIInput()
{
}

void ComponentUIInput::InputUpdate()
{

	//if()

}

void ComponentUIInput::Serialization(json& j)
{
	json _j;

	_j["Type"] = _type;
	_j["MaterialResource"] = _material->GetResourceUID();
	_j["Enabled"] = _isEnabled;
	j["Components"].push_back(_j);
}

void ComponentUIInput::DeSerialization(json& j)
{
	_material->ChangeTexture((ResourceTexture*)ModuleResourceManager::S_LoadResource(j["MaterialResource"]));

	bool enabled = j["Enabled"];
	if (!enabled)
		Disable();

	_gameObject->transform->ForceUpdate();
}

#ifdef STANDALONE
void ComponentUIInput::OnEditor()
{

	/*bool created = true;
	if (!ImGui::CollapsingHeader("Image", &created, ImGuiTreeNodeFlags_DefaultOpen)) return;
	if (!created)
	{
		_gameObject->DestroyComponent(this);
		return;
	}

	ImGui::Text("Fill Image");
	ImGui::SameLine();

	float aux1 = _fillImage;
	if (ImGui::DragFloat("##fill", &_fillImage, 0.001f, 0, 1))
	{

		if (aux1 < _fillImage)
		{
			Console::S_Log("mas");
			
			float auxPos = _fillImage - _auxFillImage;
			float auxSca = _fillImage - _auxFillImage;
			this->_gameObject->transform->SetScale({ _fillImage, this->_gameObject->transform->GetLocalScale().y, this->_gameObject->transform->GetLocalScale().z });
			this->_gameObject->transform->SetPosition({ this->_gameObject->transform->GetGlobalPosition().x + auxPos, this->_gameObject->transform->GetLocalPosition().y, this->_gameObject->transform->GetLocalPosition().z });
			_auxFillImage = _fillImage;
		}
		else
		{
			Console::S_Log("menos");
			float auxPos = _fillImage - _auxFillImage;
			this->_gameObject->transform->SetScale({  _fillImage, this->_gameObject->transform->GetLocalScale().y, this->_gameObject->transform->GetLocalScale().z });
			this->_gameObject->transform->SetPosition({ this->_gameObject->transform->GetGlobalPosition().x + auxPos, this->_gameObject->transform->GetLocalPosition().y, this->_gameObject->transform->GetLocalPosition().z });
			_auxFillImage = _fillImage;
		}

	}*/
	

}
#endif // STANDALONE