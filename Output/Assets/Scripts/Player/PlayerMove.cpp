#include "PlayerMove.h"
#include "../SwapCam.h"
HELLO_ENGINE_API_C PlayerMove* CreatePlayerMove(ScriptToInspectorInterface* script)
{
    PlayerMove* classInstance = new PlayerMove();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragFloat("Velocity", &classInstance->vel);
    script->AddDragFloat("Max Velocity", &classInstance->maxVel);
    script->AddDragFloat("Acceleration", &classInstance->accel);
    script->AddDragFloat("Brake", &classInstance->brake);
    script->AddDragBoxGameObject("Aux Cam", &classInstance->finalCam);
    script->AddDragBoxTransform("Camera player", &classInstance->MainCam);
    script->AddDragBoxTransform("Camera Starship", &classInstance->StarShipCam);
    script->AddDragBoxAnimationPlayer("Animation player", &classInstance->animationPlayer);
    script->AddDragBoxAnimationResource("Idle", &classInstance->idleAnim);
    script->AddDragBoxAnimationResource("Walk", &classInstance->walkAnim);
    return classInstance;
}

void PlayerMove::Start()
{
    aimAngle = 0.0f;
    vel = 0.0f;
    playingAnim = idleAnim;
    animationPlayer.Play();
}

void PlayerMove::Update()
{
    usingGamepad = Input::UsingGamepad();
    dt = Time::GetDeltaTime();

    //SwapCamera();

    Aim();

}

void PlayerMove::SwapCamera()
{
    SwapCam* cameraScript = (SwapCam*)finalCam.GetScript("SwapCam");

    if (cameraScript != nullptr) {
        if (cameraScript->mainCamActive) {
            cam = MainCam;
        }
        else {
            cam = StarShipCam;
        }
    }
}

void PlayerMove::Movement()
{

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