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

    bool usingGamepad;
    float dt;

    //Movement
    float vel = 1.0f;
    float currentVel = 0.0f;
    float currentInput = 0.0f; //test
    float departureTime = 0.0f;
    float secToMaxVel = 0.0f;
    float secToZeroVel = 0.0f;
    API_Transform transform;

    //Shooting
    float aimAngle;
};

