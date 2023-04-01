#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

#include "PlayerStats.h"

class SwapWeapon;

class PlayerMove : HelloBehavior
{
    enum class PlayerAnims
    {
        IDLE,
        RUN,
        DASH,
        SHOOT,
    };

public:
    void Start() override;
    void Update() override;

    void Aim();
    API_Vector2 GetMoveInput();
    float Lerp(float a, float b, float time);
    void DashSetup();
    void Dash();
    bool DashInput();

    void ShootAnim();

    bool usingGamepad;
    float dt;

    //Movement
    API_Vector2 lastMovInput = { 1.0f, 1.0f };
    API_Vector2 lastAimInput = { 0.0f, 0.0f };
    float vel = 1.0f;
    float upgradedVel = 2.0f;
    float currentVel = 0.0f;
    float currentInput = 0.0f; //test
    float departureTime = 0.0f;
    float secToMaxVel = 0.0f;
    float secToZeroVel = 0.0f;
    API_Transform transform;

    //Void Tp
    API_Vector3 initialPos;
    float yTpLimit = -50.0f;

    //Dash
    bool isDashing = false;
    float dashesAvailable;
    float maxDashCooldown;
    float maxFastDashCooldown;
    float dashCooldown;
    bool dashBuffer;
    float dashDepartTime = 0.0f;
    API_Vector3 dashInitialPos;
    API_Vector3 dashFinalPos;
    float dashTime = 0.5f;
    float dashDistance = 5.0f;
    float upgradedDashDistance = 8.0f;

    //Shooting
    float aimAngle;

    // Impulse Player
    void RecieveImpulse(API_Vector3 direction, float impulseDuration, float impulseForce);
    API_Vector3 impulseDirection;
    float impulseStrenght;
    float impulseTime;

    //Animations
    API_AnimationPlayer playerAnimator;
    uint dashAnim = 0;
    uint idleAnim = 0;
    uint runAnim = 0;
    uint shootAnim = 0;
    PlayerAnims currentAnim;
    bool isShooting = false;

    // HUD
    API_GameObject HUDGameObject;
    SwapWeapon* HUDScript;

    // skills
    API_GameObject playerStatsGO;
    PlayerStats* playerStats;
};

