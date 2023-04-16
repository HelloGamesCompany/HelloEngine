#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class ProjectilePull;

class ElectricityChain : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    void OnCollisionEnter(API::API_RigidBody other);

    bool destroy = false;

    ProjectilePull* pull;

    float damage;
    float resistanceDamage;
    int chainCount;
    float delay;
    bool triggerActive;
    API_GameObject atachedToGO;
    uint exeptionsVectorIndex;
};

