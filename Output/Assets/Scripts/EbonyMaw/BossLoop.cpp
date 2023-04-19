#include "BossLoop.h"
#include "../Player/PlayerStats.h"
#include "../Shooting/Projectile.h"
#include "../Shooting/StickBomb.h"
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
    script->AddDragBoxGameObject("Bomb", &classInstance->bomb);
    script->AddDragBoxGameObject("Bomb Shield", &classInstance->bombShield);
    script->AddDragBoxTextureResource("Texture Bomb 1", &classInstance->textureBomb[0]);
    script->AddDragBoxTextureResource("Texture Bomb 2", &classInstance->textureBomb[1]);
    script->AddDragBoxTextureResource("Texture Bomb 3", &classInstance->textureBomb[2]);
    script->AddDragBoxTextureResource("Texture Bomb 4", &classInstance->textureBomb[3]);
    script->AddDragBoxTextureResource("Texture Bomb 5", &classInstance->textureBomb[4]);
    script->AddDragBoxTextureResource("Texture Bomb 6", &classInstance->textureBomb[5]);

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

    //burn
    if (burnTime > 3.0f)
    {
        if (resetBurn >= 0.0f)
        {
            resetBurn -= Time::GetDeltaTime();
            if (resetBurn <= 0.0f)
            {
                resetBurn = 0.0f;
                burnTime -= Time::GetDeltaTime();
            }
        }
        TakeDamage(0.5f);
    }
}

void BossLoop::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionName = other.GetGameObject().GetName();

    if (hp > 0) {
        if (detectionName == "Player")
        {
            PlayerStats* pStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
            pStats->TakeDamage(meleeDmg, 0);
        }
    }
}

void BossLoop::TakeDamage(float damage)
{
    if (hp <= 0) return;

    if (canTakeDamage == true) {

        hp -= damage;
        if (hp <= maxHpLoss[phase - 1]) {
            exploting = true;
        }

    }
    else {
        shield[phase] -= damage;
    }

    if (shield[phase] <= 0) {
        phase++;
        canTakeDamage = true;
    }
}

void BossLoop::AddBomb()
{
    currentBombNum++;
    if (currentBombNum > maxBombNum) currentBombNum = maxBombNum;
    else if (currentBombNum == 1)
    {
        if (canTakeDamage) bomb.SetActive(true);
        else bombShield.SetActive(true);
    }
    if (canTakeDamage)bomb.GetMaterialCompoennt().ChangeAlbedoTexture(textureBomb[currentBombNum - 1]);
    else bombShield.GetMaterialCompoennt().ChangeAlbedoTexture(textureBomb[currentBombNum - 1]);
}

void BossLoop::CheckBombs()
{
    if (currentBombNum > 0)
    {
        StickBomb* stickBomb = (StickBomb*)bomb.GetScript("StickBomb");
        if (stickBomb == nullptr) Console::Log("StickyBomb missing in Bomb from enemy.");
        else
        {
            stickBomb->triggerActive = true;
            stickBomb->damage = 5.0f * currentBombNum;
        }
        currentBombNum = 0;
        bomb.SetActive(false);
        bombShield.SetActive(false);
    }
}

void BossLoop::AddBurn()
{
    burnTime += Time::GetDeltaTime();
    resetBurn = 0.2f;
}