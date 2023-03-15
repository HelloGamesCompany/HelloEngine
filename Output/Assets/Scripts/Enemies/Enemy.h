#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class Enemy : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    void TakeDamage(float damage, float resistanceDamage);
    void Die();

    void OnCollisionEnter(API::API_RigidBody other);

    float maxHp = 100.0f;
    float currentHp;
    float maxResistance = 100.0f;
    float currentResistance;
    float speed = 0.0f;
    float acceleration = 1.0f;
   // API_GameObject target;

    // Test Code, delete later
   // API_RigidBody rb;
};

