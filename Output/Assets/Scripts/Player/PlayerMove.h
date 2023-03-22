#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class PlayerMove : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    void Aim();
    API_Vector2 GetMoveInput();
    float Lerp(float a, float b, float time);
    void Dash();
    bool DashInput();

    bool usingGamepad;
    float dt;

    //Movement
    API_Vector2 lastMovInput = { 1.0f, 1.0f };
    API_Vector2 lastAimInput = { 0.0f, 0.0f };
    float vel = 1.0f;
    float currentVel = 0.0f;
    float currentInput = 0.0f; //test
    float departureTime = 0.0f;
    float secToMaxVel = 0.0f;
    float secToZeroVel = 0.0f;
    API_Transform transform;

    //Dash
    bool isDashing = false;
    bool dashAvailable = true;
    float dashDepartTime = 0.0f;
    API_Vector3 dashInitialPos;
    API_Vector3 dashFinalPos;
    float dashTime = 0.5f;
    float dashDistance = 5.0f;

    //Shooting
    float aimAngle;

    //Animations
    API_AnimationPlayer playerAnimator;
    uint dashAnim = 0;
};

