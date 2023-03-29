#include "Enemy.h"
#include "../Player/PlayerStats.h"
#include "../Shooting/Projectile.h"
HELLO_ENGINE_API_C Enemy* CreateEnemy(ScriptToInspectorInterface* script)
{
    Enemy* classInstance = new Enemy();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragFloat("Max Health", &classInstance->maxHp);
    script->AddDragFloat("Current Helath", &classInstance->currentHp);
    script->AddDragFloat("Resistance", &classInstance->maxResistance);
    script->AddDragFloat("Speed", &classInstance->speed);
    script->AddDragFloat("Acceleration", &classInstance->acceleration);
    script->AddDragBoxGameObject("Enemy Manager", &classInstance->enemyDropManagerGO);
   // script->AddDragBoxRigidBody("Rigidbody test", &classInstance->rb);
  //  script->AddDragBoxGameObject("Target", &classInstance->target);
    return classInstance;
}

void Enemy::Start()
{
    enemyDropManager = (EnemyDropManager*)enemyDropManagerGO.GetScript("EnemyDropManager");

    currentHp = maxHp;
    currentResistance = maxResistance;

    // TESTING CODE, DELETE LATER
   // rb.SetGravity({ 0,-0.1f,0 });
}

void Enemy::Update()
{

}

void Enemy::TakeDamage(float damage, float resistanceDamage)
{
    if (currentHp <= 0.0f) return;

    // Health damage
    currentHp -= damage;
    if (currentHp <= 0)
    {
        currentHp = 0;
        Die();
    }

    // Resistance damage
    currentResistance -= resistanceDamage;
    if (currentResistance <= 0)
    {
        currentResistance = maxResistance;
        // reaction
    }

}

void Enemy::Die()
{
    // some animation
    enemyDropManager->SpinDropRate(gameObject.GetTransform().GetGlobalPosition());
    gameObject.SetActive(false);
}

void Enemy::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Projectile")
    {
        Projectile* projectile = (Projectile*)other.GetGameObject().GetScript("Projectile");
        TakeDamage(projectile->damage, projectile->resistanceDamage);
    }
    else if(detectionTag == "Player")
    {
        PlayerStats* pStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
        pStats->TakeDamage(10);
    }
}
