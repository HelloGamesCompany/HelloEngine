#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class PlayerIndicator : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    API_GameObject player;
    API_Vector2 originIndicatorPos;
    API_Vector2 originPlayerPos;

    float movementReductionX = 1000.0f;
    float movementReductionY = 1000.0f;
    // value in LvL 1 --> x = 249.650, y = 122.530
};

