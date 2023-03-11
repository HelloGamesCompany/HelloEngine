#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class Projectile : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    void Destroy();

    float lifeTime = 5.0f;
    float damage = 0.0f;
    float resistanceDamage = 0.0f;
    API_Vector3 direction;
};

