#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class PlayerMovement : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    void SwapCamera();
    void Movement();
    void Aim();

    bool usingGamepad;
    float dt;

    float vel = 0.0f;
    float maxVel = 0.15f;
    float accel = 0.15f;
    float brake = 0.4f;
    bool moving = false;
    bool braking = false;
    bool movF = false;
    bool movB = false;
    bool movR = false;
    bool movL = false;

    API_Transform cam;
    API_GameObject finalCam;
    API_Transform MainCam;
    API_Transform StarShipCam;

    API_AnimationPlayer animationPlayer;
    uint idleAnim = 0;
    uint walkAnim = 0;
    uint currentAnim = 0;
    uint playingAnim = 0;

    float aimAngle;
};

