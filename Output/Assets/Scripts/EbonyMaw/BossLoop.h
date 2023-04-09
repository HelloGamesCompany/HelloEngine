
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
    API_GameObject rockShield;
    API_GameObject cover1;
    API_GameObject cover2;
    API_GameObject cover3;
    API_GameObject cover4;

    float dt;
    float weakTime = 0.0f;

    float time[4] = { 1000, 10.0f,7.5f,6.0f };
    float maxHpLoss[3] = { 1000.0f,500.0f,0.0f };
    float shield[3] = { 500.0f,750.0f,1000.0f };
    float maxShield[3] = { 500.0f,750.0f,1000.0f };

    float hp = 1500.0f;

    bool isBossAlive = false;
    bool isBossActive = false;
    bool canTakeDamage = false;
    int phase = 0;

    float meleeDmg = 10.0f;

    float maxResistance = 100.0f;
    float currentResistance;
    float speed = 75.0f;
    float acceleration = 1.2f;
    float currentSpeed = 0.0f;
};
