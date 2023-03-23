#include "PlayerMove.h"
#include "../SwapCam.h"
#include "../UI Test folder/SwapWeapon.h"

HELLO_ENGINE_API_C PlayerMove* CreatePlayerMove(ScriptToInspectorInterface* script)
{
    PlayerMove* classInstance = new PlayerMove();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragFloat("Velocity", &classInstance->vel);
    //script->AddDragFloat("Current Velocity", &classInstance->currentVel);
    script->AddDragFloat("SecToMaxVel", &classInstance->secToMaxVel);
    script->AddDragFloat("SecToZeroVel", &classInstance->secToZeroVel);
    //script->AddDragFloat("Current Input", &classInstance->currentInput);
    script->AddDragFloat("Dash Time", &classInstance->dashTime);
    script->AddDragFloat("Dash Distance", &classInstance->dashDistance);
    script->AddDragBoxAnimationPlayer("AnimationPlayer", &classInstance->playerAnimator);
    script->AddDragBoxAnimationResource("Dash Animation", &classInstance->dashAnim);
    script->AddDragBoxGameObject("HUD", &classInstance->HUDGameObject);
    return classInstance;
}

void PlayerMove::Start()
{
    transform = gameObject.GetTransform();
    departureTime = 0.0f;
    HUDScript = (SwapWeapon*)HUDGameObject.GetScript("SwapWeapon");
}

void PlayerMove::Update()
{
    usingGamepad = Input::UsingGamepad();
    dt = Time::GetDeltaTime();
    Aim();

    if (dashAvailable && !isDashing && DashInput())
    {
        isDashing = true;
        //dashAvailable = false; TODO: UNCOMMENT
        dashDepartTime = 0.0f;
        dashInitialPos = transform.GetLocalPosition();
        float norm = sqrt(pow(lastMovInput.x, 2) + pow(lastMovInput.y, 2));
        API_Vector3 movDir;
        movDir.x = lastMovInput.x / norm;
        movDir.y = 0.0f;
        movDir.z = lastMovInput.y / norm;
        dashFinalPos = transform.GetLocalPosition() + movDir * dashDistance; //transform.GetForward() // for looking dir

        playerAnimator.ChangeAnimation(dashAnim);
        playerAnimator.Play();
        HUDScript->Dash();
    }

    if (isDashing)
    {
        Dash();
        return; //NO MORE MOVEMENT
    }

    API_Vector2 input = GetMoveInput();
    //currentInput = input.Distance(API_Vector2::S_Zero());   //TEST

    //SecToZero MUST be smaller than SecToMaxVel
    if (abs(input.x) < 0.01f && abs(input.y) < 0.01f) //NO INPUT
    {
        if (departureTime > secToZeroVel) departureTime = secToZeroVel;

        if (departureTime > 0.0f) {
            currentVel = Lerp(0.0f, vel, departureTime / secToZeroVel);
            departureTime -= dt;
            input = lastMovInput;
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
        lastMovInput = input;
    }

    input *= currentVel * dt;
    transform.Translate(input.x, 0.0f, input.y);
}

float PlayerMove:: Lerp(float a, float b, float time)
{
    return a + time * (b - a);
}

void PlayerMove::Dash()
{
    dashDepartTime += dt;
    if (dashDepartTime > dashTime) dashDepartTime = dashTime;

    API_Vector2 newPos;
    newPos.x = Lerp(dashInitialPos.x, dashFinalPos.x, dashDepartTime / dashTime) - transform.GetLocalPosition().x;
    newPos.y = Lerp(dashInitialPos.z, dashFinalPos.z, dashDepartTime / dashTime) - transform.GetLocalPosition().z;

    //Console::Log("X: " + to_string(newPos.x));
    //Console::Log("Z: " + to_string(newPos.y));

    transform.Translate(newPos.x, 0.0f, newPos.y);
     
    if (dashDepartTime >= dashTime)
    {
        isDashing = false;
        HUDScript->Dash();
    }
}

bool PlayerMove::DashInput()
{
    if (usingGamepad)
        return Input::GetGamePadAxis(GamePadAxis::AXIS_TRIGGERLEFT) > 20000;

    return Input::GetKey(KeyCode::KEY_SPACE) == KeyState::KEY_DOWN;
}

void PlayerMove::Aim()
{
    API_Vector2 normalizedInput;
    
    if (usingGamepad)
    {
        API_Vector2 input;
        input.x = Input::GetGamePadAxis(GamePadAxis::AXIS_RIGHTX);
        input.y = -Input::GetGamePadAxis(GamePadAxis::AXIS_RIGHTY);
        
        if (abs(input.x) < 10000 && abs(input.y) < 10000) return;
        
        float norm = sqrt(pow(input.x, 2) + pow(input.y, 2));
        normalizedInput.x = input.x / norm;
        normalizedInput.y = input.y / norm;
    }
    else
    {
        normalizedInput.x = Input::GetKey(KeyCode::KEY_RIGHT) == KeyState::KEY_REPEAT;
        normalizedInput.x -= Input::GetKey(KeyCode::KEY_LEFT) == KeyState::KEY_REPEAT;
        normalizedInput.y = Input::GetKey(KeyCode::KEY_UP) == KeyState::KEY_REPEAT;
        normalizedInput.y -= Input::GetKey(KeyCode::KEY_DOWN) == KeyState::KEY_REPEAT;

        if (normalizedInput.x != 0.0f && normalizedInput.y != 0.0f) {
            // sqrt(2)/2 = 0.71
            normalizedInput.x *= 0.71f;
            normalizedInput.y *= 0.71f;
        }
    }

    //Get last input to keep Aim angle
    if (normalizedInput.x == 0.0f && normalizedInput.y == 0.0f)
        normalizedInput = lastAimInput;
    else
        lastAimInput = normalizedInput;

    aimAngle = atan2(normalizedInput.y, normalizedInput.x) * RADTODEG - 90.0f;
    gameObject.GetTransform().SetRotation(0, aimAngle, 0);
}

API_Vector2 PlayerMove::GetMoveInput()
{
    API_Vector2 input;

    //Gamepad
    if (usingGamepad) {
        input.x = Input::GetGamePadAxis(GamePadAxis::AXIS_LEFTX);
        input.y = Input::GetGamePadAxis(GamePadAxis::AXIS_LEFTY);

        if (abs(input.x) < 10000 && abs(input.y) < 10000) return API_Vector2(0.0f, 0.0f);

        if (input.x > 32000.0f) input.x = 32000.0f;
        else if (input.x < -32000.0f) input.x = -32000.0f;
        if (input.y > 32000.0f) input.y = 32000.0f;
        else if (input.y < -32000.0f) input.y = -32000.0f;

        return -input / 32000.0f;
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

void PlayerMove::OnCollisionEnter(API_RigidBody other)
{
   // Console::Log(other.GetGameObject().GetName());
}