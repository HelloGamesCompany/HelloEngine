#include "EnemyMeleeMovement.h"
#include <time.h>
#include <random>
#include <math.h>

HELLO_ENGINE_API_C EnemyMeleeMovement* CreateEnemyMeleeMovement(ScriptToInspectorInterface* script)
{
    EnemyMeleeMovement* classInstance = new EnemyMeleeMovement();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragFloat("Detection distance", &classInstance->detectionDis);
    script->AddDragFloat("Lossing Enemy distance", &classInstance->lossingDis);
    script->AddDragFloat("Range attack", &classInstance->rangeAtk);
    script->AddDragFloat("OutsideZone Time", &classInstance->outTime);
    script->AddDragFloat("Attack Time", &classInstance->attackTimeCpy);
    script->AddDragFloat("Attack Charge", &classInstance->attackChargeCpy);
    script->AddDragFloat("Attack Cooldown", &classInstance->attackCDCpy);
    script->AddDragFloat("Attack Speed", &classInstance->attackSpeed);
    script->AddDragFloat("Charge Speed", &classInstance->chargeSpeed);
    script->AddDragFloat("Walk Away Speed", &classInstance->walkAwaySpeed);
    script->AddDragBoxGameObject("Target", &classInstance->target);
    script->AddDragBoxGameObject("Action zone", &classInstance->actionZone);
    script->AddDragBoxGameObject("Attack zone", &classInstance->attackZoneGO);
    script->AddDragBoxRigidBody("Action Rb zone", &classInstance->zoneRb);
    script->AddDragBoxGameObject("Point 1", &classInstance->listPoints[0]);
    script->AddDragBoxGameObject("Point 2", &classInstance->listPoints[1]);
    script->AddDragBoxGameObject("Point 3", &classInstance->listPoints[2]);
    //script->AddDragBoxGameObject("Point 4", &classInstance->listPoints[3]);
    //script->AddDragBoxGameObject("Point 5", &classInstance->listPoints[4]);
    script->AddDragBoxAnimationPlayer("Animation Player", &classInstance->animationPlayer);
    script->AddDragBoxAnimationResource("Idle Animation", &classInstance->idleAnim);
    script->AddDragBoxAnimationResource("Walk Animation", &classInstance->walkAnim);

    return classInstance;
}

void EnemyMeleeMovement::Start()
{
    actualPoint = listPoints[0].GetTransform().GetGlobalPosition();
    _avalPoints = 3;
    enemState = States::WANDERING;
    _outCooldown = 0;
    srand(time(NULL));

    timer = 0.0;
    attackCharge = attackChargeCpy;
    attackTime = attackCharge + attackTimeCpy;
    attackCD = attackTime + attackCDCpy;

    animState = AnimationState::NONE;

    enemy = (Enemy*)gameObject.GetScript("Enemy");
    attackZone = (EnemyMeleeAttackZone*)attackZoneGO.GetScript("EnemyMeleeAttackZone");
}
void EnemyMeleeMovement::Update()
{
    
    float dt = Time::GetDeltaTime();

    if (enemy != nullptr && attackZone != nullptr)
    {
       float dis = gameObject.GetTransform().GetGlobalPosition().Distance(target.GetTransform().GetGlobalPosition());
        float disZone = gameObject.GetTransform().GetGlobalPosition().Distance(actionZone.GetTransform().GetGlobalPosition());
       //  float dis = gameObject.GetTransform().GetLocalPosition().Distance(target.GetTransform().GetGlobalPosition());
       // float disZone = gameObject.GetTransform().GetLocalPosition().Distance(actionZone.GetTransform().GetGlobalPosition());
        if (attackZone->attack)
        {
            enemState = States::ATTACKIG;
        }
        else if (dis < detectionDis && enemState != States::TARGETING)
        {
            attackZone->attack = false;
            enemState = States::TARGETING;
        }
        else if ((dis > lossingDis) /*|| ((disZone > zoneRb.GetRadius() / 2)*//* && _outCooldown >= outTime)*/)
        {
            attackZone->attack = false;
            enemState = States::WANDERING;
        }
        

        if ((disZone > zoneRb.GetRadius() / 2) /*&& enemState == States::TARGETING*/)_outCooldown += dt/*, Console::Log(std::to_string(_outCooldown))*/;
        else _outCooldown = 0;



        switch (enemState)
        {
        case States::WANDERING:
          ///  Console::Log("NumPoint: " + std::to_string(numPoint));

            enemy->currentSpeed = enemy->speed * enemy->stunVel * enemy->slowVel /** dt*/;
            
            //if ((gameObject.GetTransform().GetLocalPosition().Distance(actualPoint) < 5))
            if ((gameObject.GetTransform().GetGlobalPosition().Distance(actualPoint) < 5))
            {
                Console::Log("Change");
                numPoint++;
                if (numPoint >= _avalPoints)numPoint = 0;
            }
            actualPoint = listPoints[numPoint].GetTransform().GetGlobalPosition();
            Wander(enemy->currentSpeed, actualPoint, enemy->enemyRb);

            if (animState != AnimationState::WALK)
            {
                animState = AnimationState::WALK;
                animationPlayer.ChangeAnimation(walkAnim);
                animationPlayer.Play();
                Console::Log("Walk");
            }
            
            break;

        case States::TARGETING:
            enemy->currentSpeed = enemy->speed * enemy->acceleration * enemy->stunVel * enemy->slowVel /** dt*/;

            
            Seek(enemy->currentSpeed, target.GetTransform().GetGlobalPosition(), enemy->enemyRb);

            if (animState != AnimationState::WALK)
            {
                animState = AnimationState::WALK;
                animationPlayer.ChangeAnimation(walkAnim);
                animationPlayer.Play();
            }
            
            break;

        case States::ATTACKIG:

            if (timer < attackCharge)
            {
                ChargeAttack();
            }
            else if (timer < attackTime)
            {
                Attack();
            }
            else if (timer < attackCD)
            {
                WalkAway();
            }
            else {
                timer = 0.0f;
                attackCharge = attackChargeCpy;
                attackTime = attackCharge + attackTimeCpy;
                attackCD = attackTime + attackCDCpy;
                attackZone->attack = false;
            }

            timer += dt;
            
            ////enemy->currentSpeed = enemy->speed * enemy->acceleration * enemy->stunVel * enemy->slowVel /** dt*/;
            //if (attackCharge > 0.0f) {
            //    if (animState != AnimationState::IDLE)
            //    {
            //        animState = AnimationState::IDLE;
            //        animationPlayer.ChangeAnimation(idleAnim);
            //        animationPlayer.Play();
            //    }
            //}
            //else {
            //    if (animState != AnimationState::WALK)
            //    {
            //        animState = AnimationState::WALK;
            //        animationPlayer.ChangeAnimation(walkAnim);
            //        animationPlayer.Play();
            //    }
            //}
            
            break;
        default:
            break;
        }
    }
}


void EnemyMeleeMovement::Seek(float vel, API_Vector3 tarPos, API_RigidBody enemyRb)
{
   
    if (!enemy->actStun)
    {
        API_Vector3 _bRot = enemy->baseRot;
        API_Vector2 lookDir;
        /* lookDir.x = (point.x - gameObject.GetTransform().GetGlobalPosition().x);
        lookDir.y = (point.z - gameObject.GetTransform().GetGlobalPosition().z);*/
        lookDir.x = (tarPos.x - gameObject.GetTransform().GetLocalPosition().x);
        lookDir.y = (tarPos.z - gameObject.GetTransform().GetLocalPosition().z);

        API_Vector2 normLookDir;
        normLookDir.x = lookDir.x / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
        normLookDir.y = lookDir.y / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
        float _angle = 0;
        _angle = atan2(normLookDir.y, normLookDir.x) * RADTODEG - 90.0f;
        //gameObject.GetTransform().SetRotation(0+_bRot.x, -_angle+ _bRot.y, 0+ _bRot.z);
        gameObject.GetTransform().SetRotation(0, -_angle, 0);
    }

    enemyRb.SetVelocity(gameObject.GetTransform().GetForward() * vel);
    //gameObject.GetTransform().Translate(gameObject.GetTransform().GetForward() * vel);
}

void EnemyMeleeMovement::Wander(float vel, API_Vector3 point, API_RigidBody enemyRb)
{

    
    if (!enemy->actStun)
    {
        API_Vector3 _bRot = enemy->baseRot;
        API_Vector2 lookDir;
        /* lookDir.x = (point.x - gameObject.GetTransform().GetGlobalPosition().x);
         lookDir.y = (point.z - gameObject.GetTransform().GetGlobalPosition().z);*/
        lookDir.x = (point.x - gameObject.GetTransform().GetLocalPosition().x);
        lookDir.y = (point.z - gameObject.GetTransform().GetLocalPosition().z);

        API_Vector2 normLookDir;
        normLookDir.x = lookDir.x / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
        normLookDir.y = lookDir.y / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
        float _angle = 0;
        _angle = atan2(normLookDir.y, normLookDir.x) * RADTODEG - 90.0f;
        gameObject.GetTransform().SetRotation(0, -_angle, 0);
        //gameObject.GetTransform().SetRotation(0 + _bRot.x, -_angle - _bRot.y, 0 + _bRot.z);
    }
    enemyRb.SetVelocity(gameObject.GetTransform().GetForward() * vel);
    //gameObject.GetTransform().Translate(gameObject.GetTransform().GetForward() * vel);
}

API_Vector3 EnemyMeleeMovement::NormalizeVec3(float x, float y, float z)
{
    float lenght = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

    x = x / lenght;
    y = y / lenght;
    z = z / lenght;
    return (x, y, z);
}

float EnemyMeleeMovement::Lerp(float a, float b, float time)
{
    return a + time * (b - a);
}

void EnemyMeleeMovement::WalkAway()
{
    enemy->currentSpeed = -walkAwaySpeed;
    Seek(enemy->currentSpeed, target.GetTransform().GetGlobalPosition(), enemy->enemyRb);
}
void EnemyMeleeMovement::ChargeAttack()
{
    enemy->currentSpeed = chargeSpeed;
    Seek(enemy->currentSpeed, target.GetTransform().GetGlobalPosition(), enemy->enemyRb);
    targetPosOnAttack = target.GetTransform().GetGlobalPosition();
}
void EnemyMeleeMovement::Attack()
{
   /* if (gameObject.GetTransform().GetGlobalPosition() == targetPosOnAttack)
    {
        timer = attackTime + 2;
        attackCD += 2;
    }*/
    enemy->currentSpeed = attackSpeed;
    Seek(enemy->currentSpeed, targetPosOnAttack, enemy->enemyRb);
}