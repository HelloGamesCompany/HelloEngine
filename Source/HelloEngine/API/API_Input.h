#pragma once
#include "Globals.h"
#include "API_KeyCode.h"

enum class MouseButton
{
	LEFT= 1,
	MIDDLE = 2,
	RIGHT = 3,
	X1 = 4,
	X2 = 5
};

enum class KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

namespace API
{
	namespace Input
	{
		TO_API KeyState GetKey(KeyCode key);
		TO_API KeyState GetMouseButton(MouseButton mouseButton);
		TO_API int GetMouseX();
		TO_API int GetMouseY();
		TO_API int GetMouseZ();
		TO_API int GetMouseXMotion();
		TO_API int GetMouseYMotion();
	}
}