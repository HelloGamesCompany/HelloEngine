#include "Headers.h"
#include "API_Input.h"
#include "ModuleInput.h"
#include "LayerGame.h"

KeyState API::Input::GetKey(KeyCode key)
{
	if (!LayerGame::detectInput)
		return KeyState::KEY_IDLE;
	return (KeyState)ModuleInput::S_GetKey((int)key);
}

KeyState API::Input::GetMouseButton(MouseButton mouseButton)
{
	if (!LayerGame::detectInput)
		return KeyState::KEY_IDLE;
	return (KeyState)ModuleInput::S_GetMouseButton((int)mouseButton);
}

TO_API KeyState API::Input::GetGamePadButton(GamePadButton gamePadButton)
{
	if (!LayerGame::detectInput)
		return KeyState::KEY_IDLE;
	return (KeyState)ModuleInput::S_GetGamePadButton((GamePad)gamePadButton);
}

TO_API int API::Input::GetGamePadAxis(GamePadAxis gamePadAxis)
{
	if (!LayerGame::detectInput)
		return 0;
	return ModuleInput::S_GetGamePadAxis((SDL_GameControllerAxis)gamePadAxis);
}

int API::Input::GetMouseX()
{
	if (!LayerGame::detectInput)
		return 0;
	return ModuleInput::S_GetMouseX();
}

int API::Input::GetMouseY()
{
	if (!LayerGame::detectInput)
		return 0;
	return ModuleInput::S_GetMouseY();
}

int API::Input::GetMouseZ()
{
	if (!LayerGame::detectInput)
		return 0;
	return ModuleInput::S_GetMouseZ();
}

int API::Input::GetMouseXMotion()
{
	if (!LayerGame::detectInput)
		return 0;
	return ModuleInput::S_GetMouseXMotion();
}

int API::Input::GetMouseYMotion()
{
	if (!LayerGame::detectInput)
		return 0;
	return ModuleInput::S_GetMouseYMotion();
}
