#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class BreakableWall : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    void OnCollisionEnter(API_RigidBody other);

    void ShootWall(float projectileDamage);
    void DestroyWall();

    API_ParticleSystem wallDestroyed;

    API_GameObject fenceDestroyed;
    API_GameObject fenceEntire;

    API_RigidBody fenceRigidbody;

    int maxHp;
    int currentHp;
};

