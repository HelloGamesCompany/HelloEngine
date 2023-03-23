#pragma once
#include "Globals.h"
#include "API_KeyCode.h"

enum class MouseButton
{
    LEFT = 1,
    MIDDLE = 2,
    RIGHT = 3,
    X1 = 4,
    X2 = 5
};

enum class GamePadButton
{
    BUTTON_A,
    BUTTON_B,
    BUTTON_X,
    BUTTON_Y,
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_LEFT,
    BUTTON_RIGHT,
    BUTTON_LEFT_SHOULDER,
    BUTTON_RIGHT_SHOULDER,
    BUTTON_START,
    BUTTON_BACK,
    BUTTON_SELECT,
    BUTTON_LEFT_STICK,
    BUTTON_RIGHT_STICK
};

enum class GamePadAxis
{
    AXIS_LEFTX = 0,
    AXIS_LEFTY,
    AXIS_RIGHTX,
    AXIS_RIGHTY,
    AXIS_TRIGGERLEFT,
    AXIS_TRIGGERRIGHT,
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
        TO_API KeyState GetGamePadButton(GamePadButton gamePadButton);
        TO_API int GetGamePadAxis(GamePadAxis gamePadAxis);
        TO_API int GetMouseX();
        TO_API int GetMouseY();
        TO_API int GetMouseZ();
        TO_API int GetMouseXMotion();
        TO_API int GetMouseYMotion();
        TO_API bool UsingGamepad();
    }
}