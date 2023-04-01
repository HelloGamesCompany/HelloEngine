#include "BossMovement.h"

HELLO_ENGINE_API_C BossMovement* CreateBossMovement(ScriptToInspectorInterface* script)
{
	BossMovement* classInstance = new BossMovement();
    script->AddDragBoxGameObject("Player", &classInstance->player);
    script->AddDragBoxGameObject("Boss", &classInstance->boss);

	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void BossMovement::Start()
{
   //bAttacks = (BossAttacks*)boss.GetScript("BossAttacks");
}
void BossMovement::Update()
{
    
   //if (bAttacks->bossState != BossAttacks::BOSS_STATE::KO) angle = Rotate(player.GetTransform().GetGlobalPosition(), angle);      
    
}

float BossMovement::Rotate(API_Vector3 target, float _angle)
{
    API_Vector2 lookDir;
    lookDir.x = (target.x - gameObject.GetTransform().GetGlobalPosition().x);
    lookDir.y = (target.z - gameObject.GetTransform().GetGlobalPosition().z);

    API_Vector2 normLookDir;
    normLookDir.x = lookDir.x / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
    normLookDir.y = lookDir.y / sqrt(pow(lookDir.x, 2) + pow(lookDir.y, 2));
    _angle = 0;
    _angle = atan2(normLookDir.y, normLookDir.x) * RADTODEG - 90.0f;
    gameObject.GetTransform().SetRotation(0, -_angle, 0);

    return _angle;
}
