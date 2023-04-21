#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class CheckRicochetTargets : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    void OnCollisionEnter(API::API_RigidBody other);

    API_GameObject GetRandomTarget();

    API_GameObject atachedToGO;
    std::vector<API_GameObject> targets;
};

