#include "PlayerMove.h"
#include "../SwapCam.h"
HELLO_ENGINE_API_C PlayerMove* CreatePlayerMove(ScriptToInspectorInterface* script)
{
    PlayerMove* classInstance = new PlayerMove();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragFloat("Velocity", &classInstance->vel);
    script->AddDragFloat("Current Velocity", &classInstance->currentVel);
    script->AddDragFloat("SecToMaxVel", &classInstance->secToMaxVel);
    script->AddDragFloat("SecToZeroVel", &classInstance->secToZeroVel);
    script->AddDragFloat("Current Input", &classInstance->currentInput);

    return classInstance;
}

void PlayerMove::Start()
{
    transform = gameObject.GetTransform();
    departureTime = 0.0f;
}

void PlayerMove::Update()
{
    usingGamepad = Input::UsingGamepad();
    dt = Time::GetDeltaTime();
    Aim();

    API_Vector2 input = GetMoveInput();
    currentInput = input.Distance(API_Vector2::S_Zero());   //TEST

    //SecToZero MUST be smaller than SecToMaxVel
    if (input.x == 0.0f && input.y == 0.0f) //NO INPUT
    {
        if (departureTime > secToZeroVel) departureTime = secToZeroVel;

        if (departureTime > 0.0f) {
            currentVel = Lerp(0.0f, vel, departureTime / secToZeroVel);
            departureTime -= dt;
        }
        else {
            currentVel = 0.0f;
        }
    }
    else //MOVEMENT
    {
        if (departureTime < 0.0f) departureTime = 0.0f;

        if (departureTime < secToMaxVel) {
            currentVel = Lerp(0.0f, vel, departureTime / secToMaxVel);
            departureTime += dt;
        }
        else {
            currentVel = vel;
        }
    }

    input *= currentVel * dt;
    transform.Translate(input.x, 0, input.y);
}

float PlayerMove:: Lerp(float a, float b, float time)
{
    return a + time * (b - a);
}

void PlayerMove::Aim()
{
    if (usingGamepad)
    {
        API_Vector2 input;
        input.x = Input::GetGamePadAxis(GamePadAxis::AXIS_RIGHTX);
        input.y = -Input::GetGamePadAxis(GamePadAxis::AXIS_RIGHTY);

        if (abs(input.x) < 10000 && abs(input.y) < 10000) return;

        API_Vector2 normalizedInput;
        normalizedInput.x = input.x / sqrt(pow(input.x, 2) + pow(input.y, 2));
        normalizedInput.y = input.y / sqrt(pow(input.x, 2) + pow(input.y, 2));

        aimAngle = atan2(normalizedInput.y, normalizedInput.x) * RADTODEG - 90.0f;
        gameObject.GetTransform().SetRotation(0, aimAngle, 0);
    }
    else
    {

    }
}

API_Vector2 PlayerMove::GetMoveInput()
{
    API_Vector2 input;

    //Gamepad
    if (usingGamepad) {
        input.x = Input::GetGamePadAxis(GamePadAxis::AXIS_LEFTX);
        input.y = -Input::GetGamePadAxis(GamePadAxis::AXIS_LEFTY);

        if (abs(input.x) < 10000 && abs(input.y) < 10000) return API_Vector2::S_Zero();

        return input / 32000.0f;
    }

    //Keyboard
    input.x = Input::GetKey(KeyCode::KEY_A) == KeyState::KEY_REPEAT;
    input.x -= Input::GetKey(KeyCode::KEY_D) == KeyState::KEY_REPEAT;
    input.y = Input::GetKey(KeyCode::KEY_W) == KeyState::KEY_REPEAT;
    input.y -= Input::GetKey(KeyCode::KEY_S) == KeyState::KEY_REPEAT;

    //Diagonal movement
    if (input.x != 0.0f && input.y != 0.0f) {
        // sqrt(2)/2 = 0.71
        input.x *= 0.71f;
        input.y *= 0.71f;
    }

    return input;
}
