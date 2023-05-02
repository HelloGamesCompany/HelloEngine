#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "Enemy.h"
#include "EnemyGun.h"

#include "API/API.h"

class EnemySniper : HelloBehavior
{
    enum class AnimationState
    {
        NONE,
        IDLE,
        SHOOT,
        HITTED,
        DIE,
    };

public:
    enum class States {
        TARGETING,
        ATTACKIG,
        DYING

    };

	void Start() override; 
	void Update() override;

    void LookAt(API_Vector3 tarPos);

    float range = 0;

    API_GameObject target;
    API_GameObject gunObj;
    EnemyGun* enemyGun = nullptr;

    States enemState;

    // animation
    AnimationState animState;
    API_AnimationPlayer animationPlayer;
    uint idleAnim;
    uint aimAnim;
    uint hitAnim;
    uint dieAnim;

    Enemy* enemy = nullptr;
    PlayerStats* targStats = nullptr;
};
