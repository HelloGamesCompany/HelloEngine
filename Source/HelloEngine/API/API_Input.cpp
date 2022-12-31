#include "Headers.h"
#include "API_Input.h"
#include "ModuleInput.h"

KeyState API::Input::GetKey(KeyCode key)
{
	return (KeyState)ModuleInput::S_GetKey((int)key);
}

KeyState API::Input::GetMouseButton(MouseButton mouseButton)
{
	return (KeyState)ModuleInput::S_GetMouseButton((int)mouseButton);
}

int API::Input::GetMouseX()
{
	return ModuleInput::S_GetMouseX();
}

int API::Input::GetMouseY()
{
	return ModuleInput::S_GetMouseY();
}

int API::Input::GetMouseZ()
{
	return ModuleInput::S_GetMouseZ();
}

int API::Input::GetMouseXMotion()
{
	return ModuleInput::S_GetMouseXMotion();
}

int API::Input::GetMouseYMotion()
{
	return ModuleInput::S_GetMouseYMotion();
}
