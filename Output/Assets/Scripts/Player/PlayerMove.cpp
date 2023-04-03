#include "PlayerMove.h"
#include "../SwapCam.h"
#include "../UI Test folder/SwapWeapon.h"

HELLO_ENGINE_API_C PlayerMove* CreatePlayerMove(ScriptToInspectorInterface* script)
{
    PlayerMove* classInstance = new PlayerMove();

    script->AddDragFloat("Velocity", &classInstance->vel);
    script->AddDragFloat("Upgrade Velocity", &classInstance->upgradedVel);
    script->AddDragFloat("SecToMaxVel", &classInstance->secToMaxVel);
    script->AddDragFloat("SecToZeroVel", &classInstance->secToZeroVel);
    //script->AddDragFloat("Current Velocity", &classInstance->currentVel);
    script->AddDragFloat("Y tp limit", &classInstance->yTpLimit);
    script->AddDragBoxRigidBody("Rigid Body", &classInstance->rigidBody);

    script->AddDragFloat("Dash Time", &classInstance->dashTime);
    script->AddDragFloat("Dash Distance", &classInstance->dashDistance);
    script->AddDragFloat("Upgrade Dash Distance", &classInstance->upgradedDashDistance);
    script->AddDragFloat("Dash Cooldown", &classInstance->maxDashCooldown);
    script->AddDragFloat("Upgrade Dash Cooldown", &classInstance->maxFastDashCooldown);

    script->AddDragBoxAnimationPlayer("AnimationPlayer", &classInstance->playerAnimator);
    script->AddDragBoxAnimationResource("Dash Animation", &classInstance->dashAnim);
    script->AddDragBoxAnimationResource("Idle Animation", &classInstance->idleAnim);
    script->AddDragBoxAnimationResource("Run Animation", &classInstance->runAnim);
    script->AddDragBoxAnimationResource("Shoot Animations", &classInstance->shootAnim);

    script->AddDragBoxGameObject("Player Stats GO", &classInstance->playerStatsGO);
    return classInstance;
}

void PlayerMove::Start()
{
    transform = gameObject.GetTransform();
    initialPos = transform.GetGlobalPosition();
    departureTime = 0.0f;
    playerStats = (PlayerStats*)playerStatsGO.GetScript("PlayerStats");
    if (playerStats == nullptr) Console::Log("Missing PlayerStats on PlayerMove Script.");

    if (playerStats && playerStats->movementTreeLvl > 3) dashesAvailable = 2;
    else dashesAvailable = 1;
    dashBuffer = false;

    impulseTime = 0.0f;
} 

void PlayerMove::Update()
{
    usingGamepad = Input::UsingGamepad();
    
    //Void tp
    if (transform.GetGlobalPosition().y < yTpLimit) transform.SetPosition(initialPos);

    if (playerStats && playerStats->slowTimePowerUp > 0.0f /*&& !paused*/) dt = Time::GetRealTimeDeltaTime();
    else dt = Time::GetDeltaTime();

    Aim();

    // impulse
    if (impulseTime > 0.0f)
    {
        rigidBody.SetVelocity(impulseDirection * impulseStrenght);
        impulseTime -= dt;

        if (impulseTime <= 0.0f)
        {
            impulseTime = 0.0f;
        }
        else
        {
            return; // can't do other actions while is been impulsed
        }
    }

    if (Input::GetGamePadAxis(GamePadAxis::AXIS_TRIGGERRIGHT) < 5000)
    {
        isShooting = false;
    }

    if (dashesAvailable > 0)
    {
        if (DashInput())
        {
            if (isDashing)
            {
                dashBuffer = true;
            }
            else
            {
                DashSetup();
            }
        }
        else if (dashBuffer)
        {
            DashSetup();
            dashBuffer = false;
        }
    }

    // refresh dash
    if (dashCooldown > 0.0f)
    {
        dashCooldown -= dt;
        if (dashCooldown <= 0.0f)
        {
            dashCooldown = 0.0f;
            if (playerStats && playerStats->movementTreeLvl > 3) dashesAvailable = 2;
            else dashesAvailable = 1;
        }
    }

    if (isDashing)
    {
        Dash();
        return; //NO MORE MOVEMENT
    }

    API_Vector2 input = GetMoveInput();
    //currentInput = input.Distance(API_Vector2::S_Zero());   //TEST

    float velocity;
    if (playerStats && playerStats->movementTreeLvl > 0) velocity = upgradedVel;
    else velocity = vel;

    if (playerStats && playerStats->speedPowerUp > 0.0f) velocity *= 1.5f;
    
    //SecToZero MUST be smaller than SecToMaxVel
    if (abs(input.x) < 0.01f && abs(input.y) < 0.01f) //NO INPUT
    {
        if (departureTime > secToZeroVel) departureTime = secToZeroVel;

        if (departureTime > 0.0f) {
            currentVel = Lerp(0.0f, velocity, departureTime / secToZeroVel);
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
            currentVel = Lerp(0.0f, velocity, departureTime / secToMaxVel);
            departureTime += dt;
        }
        else {
            currentVel = velocity;
        }
        lastMovInput = input;
    }

    input *= currentVel;
    rigidBody.SetVelocity(API_Vector3(input.x, 0.0f, input.y));

    if (currentVel <= 0.0f && currentAnim != PlayerAnims::IDLE && !isShooting) //NO INPUT
    {
        playerAnimator.ChangeAnimation(idleAnim);
        playerAnimator.Play();
        currentAnim = PlayerAnims::IDLE;
    }
}

float PlayerMove:: Lerp(float a, float b, float time)
{
    return a + time * (b - a);
}

void PlayerMove::DashSetup()
{
    isDashing = true;

    // cooldown
    dashesAvailable--;
    if (playerStats && playerStats->movementTreeLvl > 1) dashCooldown = maxFastDashCooldown;
    else dashCooldown = maxDashCooldown;

    dashDepartTime = 0.0f;
    float norm = sqrt(pow(lastMovInput.x, 2) + pow(lastMovInput.y, 2));
    API_Vector3 movDir;
    movDir.x = lastMovInput.x / norm;
    movDir.y = 0.0f;
    movDir.z = lastMovInput.y / norm;

    float dist = dashDistance;
    if (playerStats && playerStats->movementTreeLvl > 2) dist = upgradedDashDistance;

    //Set dash vel
    rigidBody.SetVelocity((movDir*dist) / dashTime);

    if (currentAnim != PlayerAnims::DASH)
    {
        playerAnimator.ChangeAnimation(dashAnim);
        playerAnimator.Play();
        currentAnim = PlayerAnims::DASH;
    }
}

void PlayerMove::Dash()
{
    dashDepartTime += dt;
     
    if (dashDepartTime >= dashTime)
    {
        isDashing = false;
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

        if (currentAnim != PlayerAnims::RUN && !isShooting)
        {
            playerAnimator.ChangeAnimation(runAnim);
            playerAnimator.Play();
            currentAnim = PlayerAnims::RUN;
        }


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

void PlayerMove::ShootAnim()
{
    if (currentAnim != PlayerAnims::SHOOT)
    {
        playerAnimator.ChangeAnimation(shootAnim);
        playerAnimator.Play();
        currentAnim = PlayerAnims::SHOOT;
        isShooting = true;
    }
}

void PlayerMove::RecieveImpulse(API_Vector3 direction, float impulseDuration, float impulseForce)
{
    impulseDirection = direction;
    impulseTime = impulseDuration;
    impulseStrenght = impulseForce;
}