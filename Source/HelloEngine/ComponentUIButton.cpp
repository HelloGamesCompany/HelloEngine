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
		isFocused = true;
		if (ModuleInput::S_GetMouseButton(1) == KEY_DOWN )
		{
			State = ButtonState::ONPRESS;
		}
	}
	if (!IsMouseOver())
		isFocused = false;
	


	if (isFocused == true && ModuleInput::S_GetMouseButton(1) == KEY_DOWN)
	{
		switch (State)
		{
		case ButtonState::NORMAL:
			break;
		case ButtonState::ONPRESS:
			_gameObject->transform->SetPosition(float3{ 0,0,0 });
			break;
		default:
			break;
		}
	}
	else
	{
		_gameObject->transform->SetPosition(float3{ 0,1,0 });
	}

	/*if (IsMouseOver()) {
		isFocused = true;
		if (ModuleInput::S_GetMouseButton(1) == KEY_DOWN)
		{
			State = ButtonState::ONPRESS;
		}
	}
	if (!IsMouseOver())
		isFocused = false;

	else
	{
		State = ButtonState::NORMAL;
	}

	switch (State)
	{
	case ButtonState::NORMAL:
		Console::S_Log("Im in Normal Mode");
		//LOG("Im in Normal Mode");
		break;
	case ButtonState::ONPRESS:
		Console::S_Log("Im get Presed");
		//LOG("Im get Presed");
		break;
	default:
		break;
	}*/
}
