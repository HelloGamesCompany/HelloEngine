#include "PlayerMove.h"
#include "../SwapCam.h"
HELLO_ENGINE_API_C PlayerMove* CreatePlayerMove(ScriptToInspectorInterface* script)
{
    PlayerMove* classInstance = new PlayerMove();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragFloat("Velocity", &classInstance->vel);
    script->AddDragFloat("CurrentVelocity", &classInstance->currentInput);
   /* script->AddDragFloat("Max Velocity", &classInstance->maxVel);
    script->AddDragFloat("Acceleration", &classInstance->accel);
    script->AddDragFloat("Brake", &classInstance->brake);*/
    /*script->AddDragBoxGameObject("Aux Cam", &classInstance->finalCam);
    script->AddDragBoxTransform("Camera player", &classInstance->MainCam);
    script->AddDragBoxTransform("Camera Starship", &classInstance->StarShipCam);
    script->AddDragBoxAnimationPlayer("Animation player", &classInstance->animationPlayer);
    script->AddDragBoxAnimationResource("Idle", &classInstance->idleAnim);
    script->AddDragBoxAnimationResource("Walk", &classInstance->walkAnim);*/
    return classInstance;
}

void PlayerMove::Start()
{
    transform = gameObject.GetTransform();
}

void PlayerMove::Update()
{
    usingGamepad = Input::UsingGamepad();
    dt = Time::GetDeltaTime();

    Aim();

    API_Vector2 input = GetMoveInput();
    currentInput = input.Distance(API_Vector2::S_Zero());

    input *= vel * dt;
    transform.Translate(input.x, 0, input.y);
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
        input.x = Input::GetGamePadAxis(GamePadAxis::AXIS_RIGHTX);
        input.y = Input::GetGamePadAxis(GamePadAxis::AXIS_RIGHTY);

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
