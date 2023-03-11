#include "Enemy.h"
HELLO_ENGINE_API_C Enemy* CreateEnemy(ScriptToInspectorInterface* script)
{
    Enemy* classInstance = new Enemy();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragFloat("Health", &classInstance->maxHp);
    script->AddDragFloat("Resistance", &classInstance->maxResistance);
    script->AddDragFloat("Speed", &classInstance->speed);
    script->AddDragBoxRigidBody("Rigidbody test", &classInstance->rb);
    return classInstance;
}

void Enemy::Start()
{
    currentHp = maxHp;
    currentResistance = maxResistance;

    // TESTING CODE, DELETE LATER
    rb.SetGravity({ 0,-0.1f,0 });
}

void Enemy::Update()
{

}

void Enemy::TakeDamage(float damage, float resistanceDamage)
{
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
    gameObject.Destroy();
}

void Enemy::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionName = other.GetGameObject().GetName();
    if (detectionName == "Bullet")
    {
        gameObject.SetActive(false);
        //other.GetGameObject().SetActive(false);
    }
}
