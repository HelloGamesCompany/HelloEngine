#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
//#include "Timer.hpp"
//#include <Timer.hpp>
#include "API/API.h"

class EnemyMeleeMovement : HelloBehavior
{
    enum class AnimationState
    {
        NONE,
        IDLE,
        WALK
    };

public:
    enum class States {
        TARGETING,
        WANDERING,
        ATTACKIG

    };

    void Start() override;
    void Update() override;

    void Seek(float vel, API_Vector3 tarPos, API_RigidBody rb);
    void Wander(float vel, API_Vector3 point, API_RigidBody rb);

    API_Vector3 NormalizeVec3(float x, float y, float z);

    float Lerp(float a, float b, float time);

    bool wander = false;
    bool targeting = false;

    float detectionDis = 2.0f;
    float lossingDis = 10.0f;
    float lossingZoneDis = 200.0f;
    float rangeAtk = 60.0f;
    float outTime = 3.0f;

    API_GameObject target;
    API_GameObject actionZone;

    API_RigidBody zoneRb;

    API_GameObject listPoints[5];
    int numPoint = 0;

    API_Vector3 actualPoint;

    States enemState;

    // animation
    AnimationState animState;
    API_AnimationPlayer animationPlayer;
    uint idleAnim;
    uint walkAnim;


private:
    int _avalPoints = 0;
    float _outCooldown;//max time that can be outside the zone
    float _movCooldown;//
    //Timer clock;
};




