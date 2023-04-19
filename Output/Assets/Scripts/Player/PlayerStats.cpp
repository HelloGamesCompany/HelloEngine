#include "PlayerStats.h"
#include "PlayerMove.h"
#include "../Enemies/EnemyDrop.h"
#include "../UI Test folder/HpBar.h"
#include "../UsefulScripts/IndexContainer.h"
#include "../UI Test folder/HUD_Power_Up_Scrip.h"
HELLO_ENGINE_API_C PlayerStats* CreatePlayerStats(ScriptToInspectorInterface* script)
{
    PlayerStats* classInstance = new PlayerStats();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragFloat("Max HP", &classInstance->maxHp);
    script->AddDragFloat("Current HP", &classInstance->currentHp);
    script->AddDragFloat("Upgraded Max HP", &classInstance->upgradedMaxHp);
    script->AddDragFloat("Resistance", &classInstance->maxResistance);
    script->AddDragFloat("Deadline Percentage", &classInstance->deadlinePart);
    script->AddDragFloat("Upgraded Deadline Percentage", &classInstance->upgradedDeadlinePart);
    script->AddDragFloat("Deadline Heal Amount", &classInstance->deadlineHeal);
    script->AddDragFloat("Upgraded Deadline Heal Amount", &classInstance->upgradedDeadlineHeal);
    script->AddDragFloat("Aid Kit Heal Amount", &classInstance->aidKitHeal);
    script->AddDragFloat("Upgraded Aid Kit Heal Amount", &classInstance->upgradedAidKitHeal);
    script->AddDragBoxParticleSystem("Hit Particles", &classInstance->hitParticles);
    script->AddDragBoxParticleSystem("Passive Heal Particles", &classInstance->healParticles);
    script->AddDragBoxParticleSystem("Kid Heal Particles", &classInstance->aidKitParticles);
    script->AddDragBoxGameObject("Player GO", &classInstance->playerGO);
    script->AddDragBoxGameObject("Power Ups Managers (HUD)", &classInstance->hudPowerUpGO);
    //script->AddDragInt("movement tree lvl", &classInstance->movementTreeLvl); // use it only for playtesting
    //script->AddDragInt("armory tree lvl", &classInstance->armoryTreeLvl);
    //script->AddDragInt("health tree lvl", &classInstance->healthTreeLvl);
    //script->AddDragInt("special tree lvl", &classInstance->specialTreeLvl);
    return classInstance;
}

void PlayerStats::Start()
{
    movementTreeLvl = API_QuickSave::GetInt("tree0_level");
    armoryTreeLvl = API_QuickSave::GetInt("tree1_level");
    healthTreeLvl = API_QuickSave::GetInt("tree2_level");
    specialTreeLvl = API_QuickSave::GetInt("tree3_level");

    if (healthTreeLvl > 0) currentMaxHp = upgradedMaxHp;
    else currentMaxHp = maxHp;
    currentHp = currentMaxHp;
    currentResistance = maxResistance;
    playingHealParticles = false;

    detected = false;

    if (healthTreeLvl > 4) secondLife = true;
    else secondLife = false;

    speedPowerUp = 0;
    fireratePowerUp = 0;
    shield = 0;
    slowTimePowerUp = 0;

    storage = (PlayerStorage*)playerGO.GetScript("PlayerStorage");
    if (!storage) Console::Log("Storage Missing in PlayerStats. Only needed in levels.");

    playerMove = (PlayerMove*)playerGO.GetScript("PlayerMove");
    if (!playerMove) Console::Log("PlayerMove Missing in PlayerStats.");

    hudPowerUp = (HUD_Power_Up_Scrip*)hudPowerUpGO.GetScript("HUD_Power_Up_Scrip");
    if (!hudPowerUp) Console::Log("HUD_Power_Up_Scrip Missing in PlayerStats. Only needed in levels.");
}

void PlayerStats::Update()
{
    float dt;
    if (slowTimePowerUp > 0.0f /*&& !paused*/) dt = Time::GetRealTimeDeltaTime();
    else dt = Time::GetDeltaTime();

    // deadline healing
    float deathlineHp;
    if (healthTreeLvl > 1) deathlineHp = currentMaxHp * (upgradedDeadlinePart / 100.0f);
    else deathlineHp = currentMaxHp * (deadlinePart / 100.0f);
    if (currentHp < deathlineHp)
    {
        lastHitTime -= dt;
        if (lastHitTime <= 0.0f)
        {
            if (healthTreeLvl > 3) currentHp += upgradedDeadlineHeal;
            else currentHp += deadlineHeal;

            if (currentHp > deathlineHp)
            {
                currentHp = deathlineHp;
                lastHitTime = 0.0f;
                if (playingHealParticles)
                {
                    healParticles.StopEmitting();
                    playingHealParticles = false;
                }
            }
            else
            {
                lastHitTime = 1.0f; // heal each second
                if (!playingHealParticles)
                {
                    healParticles.Play();
                    playingHealParticles = true;
                }
            }
        }
    }

    if (hittedTime > 0.0f)
    {
        hittedTime -= dt;
    }
    if (deathTime > 0.0f)
    {
        deathTime -= dt;
        if (deathTime <= 0.0f) Scene::LoadScene("LoseMenu.HScene");
    }
    if (inmunityTime > 0.0f)
    {
        inmunityTime -= dt;
    }

    // power ups
    if (speedPowerUp > 0.0f)
    {
        speedPowerUp -= dt;
        if (speedPowerUp <= 0.0f) speedPowerUp = 0.0f;
    }
    if (fireratePowerUp > 0.0f)
    {
        fireratePowerUp -= dt;
        if (fireratePowerUp <= 0.0f) fireratePowerUp = 0.0f;
    }
    if (slowTimePowerUp > 0.0f)
    {
        slowTimePowerUp -= dt;
        if (slowTimePowerUp <= 0.0f)
        {
            slowTimePowerUp = 0.0f;
            Time::ChangeTimeScale(1.0f);
        }
    }
}

void PlayerStats::OnCollisionEnter(API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();

    if (detectionTag == "EnemyDrop")
    {
        EnemyDrop* enemyDrop = (EnemyDrop*)other.GetGameObject().GetScript("EnemyDrop");

        switch (enemyDrop->dropIndex)
        {
        case 0: // laser ammo
            GetAmmo(1, 100);
            break;
        case 1: // first aid kit
            if (healthTreeLvl > 3) Heal(upgradedAidKitHeal);
            else Heal(aidKitHeal);
            break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            GetPowerUp(enemyDrop->dropIndex - 2);
            break;
        default:
            Console::Log("Enemy Drop with wrong index.");
            break;
        }

        enemyDrop->Destroy();
    }
    else if (detectionTag == "Casette")
    {
        IndexContainer* indexContainer = (IndexContainer*)other.GetGameObject().GetScript("IndexContainer");
        if (!indexContainer)
        {
            Console::Log("Casette needs IndexContainer Script.");
            return;
        }
        if (!storage)
        {
            Console::Log("Storage missing in PlayerStats Script.");
            return;
        }

        switch (indexContainer->index)
        {
        case 1:
            storage->casette1Picked = true;
            break;
        case 2:
            storage->casette2Picked = true;
            break;
        case 3:
            storage->casette3Picked = true;
            break;
        default:
            Console::Log("Casette index only can be 1, 2 or 3.");
            break;
        }
        other.GetGameObject().SetActive(false);
    }
    else if (detectionTag == "CheckPoint")
    {
        if (!storage)
        {
            Console::Log("Storage missing in PlayerStats Script.");
            return;
        }

        storage->SaveData();
    }
}

void PlayerStats::TakeDamage(float amount, float resistanceDamage)
{
    if (inmunityTime > 0.0f || currentHp <= 0) return; // only VS2

    float shieldBefore = shield;
    shield -= amount;
    if (shield <= 0.0f)
    {
        currentHp += shield;
        shield = 0.0f;
        if (shieldBefore > 0) hudPowerUp->RemovePowerUp(PowerUp_Type::SHIELD);
    }

    // hp damage
    if (currentHp <= 0)
    {
        if (secondLife)
        {
            secondLife = false;
            currentHp = 1;
            inmunityTime = 1.0f;
            Audio::Event("starlord_damaged"); // second life audio
        }
        else
        {
            currentHp = 0;
            Audio::Event("starlord_dead");
            deathTime = 1.5f;
            if (playerMove) playerMove->PlayDeathAnim();
            return;
        }
    }
    else
    {
        inmunityTime = 1.0f;
        Audio::Event("starlord_damaged");
        hitParticles.Play();
    }

    // Resistance damage
    currentResistance -= resistanceDamage;
    if (currentResistance <= 0.0f)
    {
        currentResistance = maxResistance;
        hittedTime = 0.5f;
        if (playerMove) playerMove->PlayHittedAnim();
    }

    lastHitTime = 3.0f; // 3 seg to auto heal after a hit
    if (playingHealParticles)
    {
        healParticles.StopEmitting();
        playingHealParticles = false;
    }
}

void PlayerStats::Heal(float amount)
{
    currentHp += amount;
    aidKitParticles.Play();

    Audio::Event("heal");

    if (currentHp > currentMaxHp) currentHp = currentMaxHp;
}

int PlayerStats::GetAmmonByType(int type)
{
    switch (type)
    {
    case 0:
        return 1; // infinite base ammo
        break;
    case 1:
        return laserAmmo;
        break;
    case 2:
    case 3:
        return specialAmmo;
        break;
    default:
        Console::Log("Invalid type, type can only be 0, 1, 2 or 3.");
        return -1;
        break;
    }
}

void PlayerStats::GetAmmo(int type, int amount)
{
    switch (type)
    {
    case 1:
        laserAmmo += amount;
        if (laserAmmo > maxLaserAmmo) laserAmmo = maxLaserAmmo;
        break;
    case 2:
        specialAmmo += amount;
        if (specialAmmo > maxFireAmmo) specialAmmo = maxFireAmmo;
        break;
    case 3:
        specialAmmo += amount;
        if (specialAmmo > maxRicochetAmmo) specialAmmo = maxRicochetAmmo;
        break;
    default:
        Console::Log("Invalid type, can only get ammo of types 1, 2 or 3.");
        break;
    }
}

void PlayerStats::UseAmmo(int type, int amount)
{
    switch (type)
    {
    case 1:
        laserAmmo -= amount;
        break;
    case 2:
    case 3:
        specialAmmo -= amount;
        break;
    default:
        Console::Log("Invalid type, can only use ammo of types 1, 2 or 3.");
        break;
    }
}

void PlayerStats::UpgradeTreeLvl(int tree)
{
    switch (tree)
    {
    case 0:
        movementTreeLvl++;
        break;
    case 1:
        armoryTreeLvl++;
        break;
    case 2:
        healthTreeLvl++;
        break;
    case 3:
        specialTreeLvl++;
        break;
    default:
        Console::Log("Invalid tree index, can only be 0, 1, 2 or 3.");
        break;
    }
}

void PlayerStats::GetPowerUp(int index)
{
    if (hudPowerUp == nullptr) return;

    switch (index)
    {
    case 0:
        speedPowerUp = 5.0f;
        hudPowerUp->AddPowerUp(PowerUp_Type::SPEED_INCREASE, speedPowerUp);
        break;
    case 1:
        fireratePowerUp = 5.0f;
        hudPowerUp->AddPowerUp(PowerUp_Type::FIRERATE_INCREASE, fireratePowerUp);
        break;
    case 2:
        shield = 50.0f;
        hudPowerUp->AddPowerUp(PowerUp_Type::SHIELD, 1);
        break;
    case 3:
        GetAmmo(1, 9999);
        GetAmmo(2, 9999);
        GetAmmo(3, 9999);
        break;
    case 4:
        slowTimePowerUp = 5.0f;
        Time::ChangeTimeScale(0.5f);
        hudPowerUp->AddPowerUp(PowerUp_Type::SLOW_TIME, slowTimePowerUp);
        break;
    default:
        Console::Log("Invalid powe up index, can only be 0, 1, 2 or 3.");
        break;
    }

    Audio::Event("power_up");
}

void PlayerStats::SaveInStorage(int index)
{
    if (!storage)
    {
        Console::Log("Storage missing in PlayerStats Script.");
        return;
    }

    switch (index)
    {
    case 0:
        storage->upgradeBlueprintAmount++;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        storage->unlockGunBlueprint = index;
        break;
    
    default:
        break;
    }

    storage->SaveData();
}

void PlayerStats::SaveChestData(int chestContent, int chestIndex)
{
    storage->SaveDataFromChest(chestIndex, chestContent);

    SaveInStorage(chestContent);
}