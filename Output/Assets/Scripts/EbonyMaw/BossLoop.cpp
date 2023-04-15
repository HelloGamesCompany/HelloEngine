#include "BossLoop.h"
#include "../Player/PlayerStats.h"
#include "../Shooting/Projectile.h"
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
    script->AddDragBoxGameObject("PLAYER", &classInstance->player);
    script->AddDragBoxGameObject("SHIELD", &classInstance->rockShield);
    script->AddDragBoxGameObject("Cover1", &classInstance->cover1);
    script->AddDragBoxGameObject("Cover2", &classInstance->cover2);
    script->AddDragBoxGameObject("Cover3", &classInstance->cover3);
    script->AddDragBoxGameObject("Cover4", &classInstance->cover4);
    script->AddDragBoxGameObject("Cover5", &classInstance->cover5);
    script->AddDragBoxGameObject("Cover6", &classInstance->cover6);
    script->AddDragBoxGameObject("Cover7", &classInstance->cover7);
    script->AddDragBoxGameObject("Cover8", &classInstance->cover8);
    script->AddDragBoxGameObject("Cover9", &classInstance->cover9);
    script->AddDragBoxGameObject("Cover10", &classInstance->cover10);
    script->AddDragBoxGameObject("Cover11", &classInstance->cover11);
    script->AddDragBoxGameObject("Cover12", &classInstance->cover12);

    //TEMPORAL FOR ALPHA 1
    script->AddDragBoxGameObject("TEMPORAL- finalTextPanel", &classInstance->finalTextPanel);

    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    return classInstance;
}

void BossLoop::Start()
{
}

void BossLoop::Update()
{
    dist = player.GetTransform().GetGlobalPosition().Distance(gameObject.GetTransform().GetGlobalPosition());

    if (dist < 80.0f) {
        if (hp > 0) {
            if (canTakeDamage == true) {
                dt = Time::GetDeltaTime();
                weakTime += dt;
                rockShield.SetActive(false);
            }
            else {
                rockShield.SetActive(true);
            }
            if (weakTime >= time[phase]) {
                weakTime = 0;
                phase--;
                shield[phase] = maxShield[phase];
                canTakeDamage = false;
            }
            if (hp <= maxHpLoss[phase - 1]) {
                weakTime = 0;
                canTakeDamage = false;
                if (phase == 2) {
                    cover1.SetActive(false);
                    cover2.SetActive(false);
                    cover3.SetActive(false);
                    cover4.SetActive(false);
                    cover5.SetActive(false);
                    cover6.SetActive(false);
                    cover7.SetActive(false);
                    cover8.SetActive(false);
                    cover9.SetActive(false);
                    cover10.SetActive(false);
                    cover11.SetActive(false);
                    cover12.SetActive(false);
                }
            }
        }
        else {
            gameObject.SetActive(false);
            gameObject.GetTransform().SetScale(0, 0, 0);
            //TEMPORAL FOR ALPHA 1
            finalTextPanel.SetActive(true);
            API_QuickSave::SetBool("level3_completed", true);
        }
    }

}

void BossLoop::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionName = other.GetGameObject().GetName();

    if (hp > 0) {
        if (detectionName == "Projectile")
    {
        other.GetGameObject().GetTransform().SetPosition(other.GetGameObject().GetTransform().GetLocalPosition().x, 1000000, other.GetGameObject().GetTransform().GetLocalPosition().z);
        Projectile* projectile = (Projectile*)other.GetGameObject().GetScript("Projectile");

        if (canTakeDamage == true) {

            hp -= projectile->damage;
            if (hp <= maxHpLoss[phase - 1]) {
                exploting = true;
            }

        }
        else {
            shield[phase] -= projectile->damage;
        }

        if (shield[phase] <= 0) {
            phase++;
            canTakeDamage = true;
        }
    }
    else if (detectionName == "Player")
    {
        PlayerStats* pStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
        pStats->TakeDamage(meleeDmg, 0);
    }
    }
}
