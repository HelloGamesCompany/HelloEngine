#include "Headers.h"
#include "ComponentUISlider.h"
#include "GameObject.h"
#include "MaterialComponent.h"
#include "ImWindowGame.h"
#include "LayerEditor.h"

ComponentUISlider::ComponentUISlider(GameObject* gameObject) : ComponentUI(gameObject)
{
	_type = Component::Type::UI_SLIDER;

	State = SliderState::NORMAL;
	
	_gameWindow = (ImWindowGame*)LayerEditor::_imWindows[(uint)ImWindowID::GAME];



	if (gameObject->GetTag() == "UIsliderBar")
	{
		gameObject->transform->SetScale({ 0.7f,0.1f,0});
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
	//double normalizedX = -1.0 + 2.0 * (double)Mouse.getX() / window.width;
	//double normalizedY = 1.0 - 2.0 * (double)Mouse.getY() / window.height;

	float windowPos = { (float)_gameWindow->gamePosX };
	normalizedPos = { (float)ModuleInput::S_GetMouseX()};
	//normalizedPos = { normalizedPos - windowPos };
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
			if (ModuleInput::S_GetMouseButton(1) == KEY_REPEAT) 
			{
				if (ModuleInput::S_GetKey(SDL_SCANCODE_D) == KEY_DOWN) 
				{
					if (mousePosX < 1.0f) 
					{
						mousePosX += 0.1f;
					}
				}
				else if (ModuleInput::S_GetKey(SDL_SCANCODE_A) == KEY_DOWN)
				{
					if (mousePosX > -1.0f)
					{
						mousePosX -= 0.1f;
					}
				}
			}
			_gameObject->transform->SetPosition({ mousePosX, 0, 0 });
			//LOG("Im get Presed");
			break;
		default:
			break;
		}

		if (!IsMouseOver())
			isFocused = false;

		if (ModuleInput::S_GetMouseButton(1) == KEY_UP)
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
	_j["PositionButton"] = normalizedPos;
	_j["StateButton"] = State;
	_j["Enabled"] = _isEnabled;
	_j["ScaleX"] = _gameObject->transform->GetLocalScale().x;
	_j["ScaleY"] = _gameObject->transform->GetLocalScale().y;
	_j["ScaleZ"] = _gameObject->transform->GetLocalScale().z;
	j["Components"].push_back(_j);
}

void ComponentUISlider::DeSerialization(json& j)
{
	_material->ChangeTexture((ResourceTexture*)ModuleResourceManager::S_LoadResource(j["MaterialResource"]));

	bool enabled = j["Enabled"];
	if (!enabled)
		Disable();
	
	normalizedPos = j["PositionButton"];
	State = j["StateButton"];

	_gameObject->transform->SetScale({ j["ScaleX"], j["ScaleY"], j["ScaleZ"] });

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