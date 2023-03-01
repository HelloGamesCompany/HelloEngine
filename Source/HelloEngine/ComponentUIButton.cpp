#include "Headers.h"
#include "ComponentUIButton.h"
#include "GameObject.h"
#include "MaterialComponent.h"

ComponentUIButton::ComponentUIButton(GameObject* gameObject) : ComponentUI(gameObject)
{
	State = ButtonState::NORMAL;
	_type = Component::Type::UI_BUTTON;
}

ComponentUIButton::~ComponentUIButton()
{
}

void ComponentUIButton::InputUpdate()
{
	// Add here any checks necessary with INPUT.

	if (IsMouseOver()) {
		//esta seleccionat
		if (State != ButtonState::ONHOLD && ModuleInput::S_GetMouseButton(1) != KEY_REPEAT )
		{
			State = ButtonState::HOVERED;
			gameTimeCopy = EngineTime::GameTimeCount();
		}

		//ha sigut clicat
		if (ModuleInput::S_GetMouseButton(1) == KEY_UP && State != ButtonState::ONHOLD)
		{
			State = ButtonState::ONPRESS;
		}
		//esta sent mantenit clickat
		if (ModuleInput::S_GetMouseButton(1) == KEY_REPEAT)
		{
			if (EngineTime::GameTimeCount() >= gameTimeCopy + 0.5)
			{
				State = ButtonState::ONHOLD;
			}
		}

		else if(State == ButtonState::ONHOLD)
		{
			State = ButtonState::HOVERED;
		}
	}

	

	switch (State)
	{
	case ButtonState::NORMAL:
		Console::S_Log("Im in Normal Mode");
		//LOG("Im in Normal Mode");
		break;
	case ButtonState::HOVERED:
		Console::S_Log("Im Hovered");
		break;
	case ButtonState::ONPRESS:
		Console::S_Log("Im get Presed");
		//LOG("Im get Presed");
		break;
	case ButtonState::ONHOLD:
		Console::S_Log("Im Holded");
		//LOG("Im get Presed");
		break;
	default:
		break;
	}

	if (!IsMouseOver())
	{
		State = ButtonState::NORMAL;
		gameTimeCopy = EngineTime::GameTimeCount();
	}
}

void ComponentUIButton::Serialization(json& j)
{
	json _j;

	_j["Type"] = _type;
	_j["MaterialResource"] = _material->GetResourceUID();
	_j["Enabled"] = _isEnabled;
	_j["State"] = State;
	j["Components"].push_back(_j);
}

void ComponentUIButton::DeSerialization(json& j)
{
	_material->ChangeTexture((ResourceTexture*)ModuleResourceManager::S_LoadResource(j["MaterialResource"]));

	bool enabled = j["Enabled"];
	if (!enabled)
		Disable();

	_gameObject->transform->ForceUpdate();

	State = j["State"];
}
