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

    void ActiveSlow(float q, float time);//q how slower the enemy will be
   
    void ActiveStun(float time);

    void EnemySlow(float xSlow, float time);//how slower the enemy will be
    void EnemyRecoil(float xRec);
    void EnemyStun(float timeStun);


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
    bool hasShield = false;
   // API_GameObject target;

    // Test Code, delete later
   // API_RigidBody rb;

    bool actSlow;
    bool actStun;
    
    API_Vector3 baseRot;//base rotation object

    float stunVel;
    float slowVel;

    API_ParticleSystem hitParticles;

    bool isOut;
    bool isHit;
    bool isTargIn;

private:
    float _coldStun;
    float _coldSlow;

    float _tSlow;
    float _qSlow;
    float _tStun;
};

