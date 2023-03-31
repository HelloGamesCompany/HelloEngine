
#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class BossLoop : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    void OnCollisionEnter(API::API_RigidBody other);

    API_GameObject boss;

    float dt;
    float weakTime = 0.0f;

    float time[4] = { 1000, 7.5f,5.0f,4.0f };
    float maxHpLoss[3] = { 100.0f,50.0f,0.0f };
    float shield[3] = { 50.0f,50.0f,50.0f };

    float hp = 200.0f;

    bool isBossAlive = false;
    bool isBossActive = false;
    bool canTakeDamage = false;
    int phase = 0;

    float damage = 5.0f;
    float meleeDmg = 10.0f;

    float maxResistance = 100.0f;
    float currentResistance;
    float speed = 75.0f;
    float acceleration = 1.2f;
    float currentSpeed = 0.0f;
};
