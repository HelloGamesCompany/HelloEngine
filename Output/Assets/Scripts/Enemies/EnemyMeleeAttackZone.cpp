#include "EnemyMeleeAttackZone.h"
HELLO_ENGINE_API_C EnemyMeleeAttackZone* CreateEnemyMeleeAttackZone(ScriptToInspectorInterface* script)
{
	EnemyMeleeAttackZone* classInstance = new EnemyMeleeAttackZone();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddCheckBox("attack", &classInstance->attack);
	return classInstance;
}

void EnemyMeleeAttackZone::Start()
{
    //attack = false;
}
void EnemyMeleeAttackZone::Update()
{

}

void EnemyMeleeAttackZone::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Projectile")
    {

    }
    else if (detectionTag == "Player")
    {
        attack = true;
    }
}