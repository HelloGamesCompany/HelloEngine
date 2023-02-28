#include "Headers.h"
#include "ComponentUIButton.h"
#include "GameObject.h"

ComponentUIButton::ComponentUIButton(GameObject* gameObject) : ComponentUI(gameObject)
{
	State = ButtonState::NORMAL;
}

ComponentUIButton::~ComponentUIButton()
{
}

void ComponentUIButton::InputUpdate()
{
	// Add here any checks necessary with INPUT.

	if (IsMouseOver()) {
		if (State != ButtonState::ONHOLD)
		{
			State = ButtonState::HOVERED;
		}

		if (ModuleInput::S_GetMouseButton(1) == KEY_UP)
		{
			State = ButtonState::ONPRESS;
		}

		if (ModuleInput::S_GetMouseButton(1) == KEY_REPEAT)
		{
			State = ButtonState::ONHOLD;
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
	}
}
