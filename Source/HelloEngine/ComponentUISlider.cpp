#include "Headers.h"
#include "ComponentUISlider.h"
#include "GameObject.h"
#include "MaterialComponent.h"
#include "ImWindowGame.h"
#include "LayerEditor.h"


ComponentUISlider::ComponentUISlider(GameObject* gameObject) : ComponentUI(gameObject)
{
	State = SliderState::NORMAL;
	float widthSlider = 0.7f;
	float heightSlider = 0.1f;
	float buttonSliderPosX= 0.0f;

	//_gameWindow = (ImWindowGame*)LayerEditor::_imWindows[(uint)ImWindowID::GAME];

	float3 buttonSliderPos = {buttonSliderPosX,0,0 };

	if (gameObject->GetTag() == "UIsliderBar")
	{
		gameObject->transform->SetScale({widthSlider,heightSlider,0});
	}
	if (gameObject->GetTag() == "UIsliderButton")
	{
		gameObject->transform->SetScale({ 0.2,0.2,0 });
	}

}

ComponentUISlider::~ComponentUISlider()
{
}

void ComponentUISlider::InputUpdate()
{
	float windowPos = { (float)_gameWindow->gamePosX };
	float normalizedPos = { (float)ModuleInput::S_GetMouseX()};
	normalizedPos = { normalizedPos - windowPos };
	//_gameObject->transform->SetScale(widthSlider, heightSlider, heightSlider);

	if (_gameObject->GetTag() == "UIsliderButton") {

		if (IsMouseOver()) {
			isFocused = true;
			if (ModuleInput::S_GetMouseButton(1) == KEY_DOWN)
			{
				State = SliderState::ONPRESS;
			}
		}

		switch (State)
		{
		case SliderState::NORMAL:
			Console::S_Log("Im in Normal Mode");
			//LOG("Im in Normal Mode");
			break;
		case SliderState::ONPRESS:
			Console::S_Log("Im get Presed");
			_gameObject->transform->SetPosition({normalizedPos, 0, 0 });
			//LOG("Im get Presed");
			break;
		default:
			break;
		}

		if (!IsMouseOver())
			isFocused = false;

		else if (ModuleInput::S_GetMouseButton(1) == KEY_UP)
		{
			State = SliderState::NORMAL;
		}
	}
}

void ComponentUISlider::Serialization(json& j)
{
	json _j;

	_j["Type"] = _type;
	_j["MaterialResource"] = _material->GetResourceUID();
	//_j["widthSlider"] = widthSlider;
	//_j["heightSlider"] = heightSlider;
	_j["Enabled"] = _isEnabled;
	j["Components"].push_back(_j);
}

void ComponentUISlider::DeSerialization(json& j)
{
	_material->ChangeTexture((ResourceTexture*)ModuleResourceManager::S_LoadResource(j["MaterialResource"]));

	bool enabled = j["Enabled"];
	if (!enabled)
		Disable();

	_gameObject->transform->ForceUpdate();
}

#ifdef STANDALONE
void ComponentUISlider::OnEditor()
{
	bool created = true;
	if (!ImGui::CollapsingHeader("Slider", &created, ImGuiTreeNodeFlags_DefaultOpen)) return;
	if (!created)
	{
		_gameObject->DestroyComponent(this);
		return;
	}

	/*bool auxiliaryBool = _isEnabled;
	if (ImGui::Checkbox("Active##Material", &auxiliaryBool))
		auxiliaryBool ? Enable() : Disable();*/

	ImGui::Text("Im a Slider");

}
#endif // STANDALONE