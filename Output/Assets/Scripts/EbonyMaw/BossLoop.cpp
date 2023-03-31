#include "BossLoop.h"
#include "../Player/PlayerStats.h"

//Pau Olmos

HELLO_ENGINE_API_C BossLoop* CreateBossLoop(ScriptToInspectorInterface* script)
{
    BossLoop* classInstance = new BossLoop();
    script->AddDragFloat("hp", &classInstance->hp);
    script->AddDragInt("Phase", &classInstance->phase);
    script->AddDragFloat("Weakness", &classInstance->weakTime);
    script->AddDragFloat("shield1Hp", &classInstance->shield[0]);
    script->AddDragFloat("shield2Hp", &classInstance->shield[1]);
    script->AddDragFloat("shield3Hp", &classInstance->shield[2]);

    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    return classInstance;
}

void BossLoop::Start()
{

}
void BossLoop::Update()
{
    

    if (hp > 0) {
if (canTakeDamage == true) {
        dt = Time::GetDeltaTime();
        weakTime += dt;
    }
    if (weakTime >= time[phase]) {
        weakTime = 0;
        phase--;
        shield[phase] = 50.0f;
        canTakeDamage = false;
    }
    if (hp <= maxHpLoss[phase - 1]) {
        weakTime = 0;
        canTakeDamage = false;
    }
    }
    else {
        gameObject.SetActive(false);
        gameObject.GetTransform().SetScale(0, 0, 0);
    }

}

void BossLoop::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionName = other.GetGameObject().GetName();
    //damage = other.GetGameObject().damage;
    if (hp > 0) {
        if (detectionName == "Projectile")
    {
        other.GetGameObject().GetTransform().SetPosition(other.GetGameObject().GetTransform().GetLocalPosition().x, 1000000, other.GetGameObject().GetTransform().GetLocalPosition().z);
        if (canTakeDamage == true) {
            hp -= damage;
        }
        else {
            shield[phase] -= damage / 2;
        }

        if (shield[phase] <= 0) {
            phase++;
            canTakeDamage = true;
        }
    }
    else if (detectionName == "Player")
    {
        PlayerStats* pStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
        pStats->TakeDamage(meleeDmg);


    }
    }
    

}
