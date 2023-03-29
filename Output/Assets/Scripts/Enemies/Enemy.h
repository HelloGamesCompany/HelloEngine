#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

#include "EnemyDropManager.h"

class Enemy : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    void TakeDamage(float damage, float resistanceDamage);
    void Die();

    void OnCollisionEnter(API::API_RigidBody other) override;

    API_GameObject enemyDropManagerGO;
    //EnemyDropManager* enemyDropManager;
    API_RigidBody enemyRb;

    float maxHp = 100.0f;
    float currentHp;
    float maxResistance = 100.0f;
    float currentResistance;
    float speed = 75.0f;
    float acceleration = 1.2f;
    float currentSpeed = 0.0f;
   // API_GameObject target;

    // Test Code, delete later
   // API_RigidBody rb;
};

