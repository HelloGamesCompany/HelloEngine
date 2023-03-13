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

    bool usingGamepad;
    float dt;

    //Movement
    float vel = 1.0f;
    float currentInput = 0.0f; //test
    API_Transform transform;

    //Shooting
    float aimAngle;
};

