#include "PlayerStats.h"
#include "../Enemies/EnemyDrop.h"
#include "../UI Test folder/HpBar.h"
#include "../UsefulScripts/IndexContainer.h"
HELLO_ENGINE_API_C PlayerStats* CreatePlayerStats(ScriptToInspectorInterface* script)
{
    PlayerStats* classInstance = new PlayerStats();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragFloat("Max HP", &classInstance->maxHp);
    script->AddDragFloat("Current HP", &classInstance->currentHp);
    script->AddDragFloat("Upgraded Max HP", &classInstance->upgradedMaxHp);
    script->AddDragInt("Laser Ammo", &classInstance->laserAmmo);
    script->AddDragInt("Fire Ammo", &classInstance->fireAmmo);
    script->AddDragInt("Ricochet Ammo", &classInstance->ricochetAmmo);
    script->AddDragBoxGameObject("Storage GO", &classInstance->storageGameObject);
    //script->AddDragBoxGameObject("Health bar", &classInstance->hpGameObject);
    script->AddDragInt("movement tree lvl", &classInstance->movementTreeLvl); // remove when save and load is ready
    script->AddDragInt("armory tree lvl", &classInstance->armoryTreeLvl);
    script->AddDragInt("health tree lvl", &classInstance->healthTreeLvl);
    script->AddDragInt("special tree lvl", &classInstance->specialTreeLvl);
    return classInstance;
}

void PlayerStats::Start()
{
    if (healthTreeLvl > 0) currentMaxHp = upgradedMaxHp;
    else currentMaxHp = maxHp;
    currentHp = currentMaxHp;
    //healthBar = (HpBar*)hpGameObject.GetScript("HpBar");
    //healthBar->maxHp = this->currentHp;
    detected = false;

    if (healthTreeLvl > 4) secondLife = true;
    else secondLife = false;

    speedPowerUp = 0;
    fireratePowerUp = 0;
    shield = 0;
    slowTimePowerUp = 0;

    storage = (PlayerStorage*)storageGameObject.GetScript("PlayerStorage");
    if (!storage) Console::Log("Storage Missing in PlayerStats. Only needed in levels.");
}

void PlayerStats::Update()
{
    float dt;
    if (slowTimePowerUp > 0.0f /*&& !paused*/) dt = Time::GetRealTimeDeltaTime();
    else dt = Time::GetDeltaTime();

    //healthBar->hp = this->currentHp;

    // deadline healing
    float deathlineHp;
    if (healthTreeLvl > 1) deathlineHp = currentMaxHp * 0.25f;
    else deathlineHp = currentMaxHp * 0.20f;
    if (currentHp < deathlineHp)
    {
        lastHitTime -= dt;
        if (lastHitTime <= 0.0f)
        {
            if (healthTreeLvl > 3) currentHp += 7.5f;
            else currentHp += 5.0f;
            
            if (currentHp > deathlineHp)
            {
                currentHp = deathlineHp;
                lastHitTime = 0.0f;
            }
            else
            {
                lastHitTime = 1.0f;
            }
        }
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
            Heal(50.0f);
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

void PlayerStats::TakeDamage(float amount)
{
    if (inmunityTime > 0.0f) return; // only VS2

    shield -= amount;
    if (shield <= 0.0f)
    {
        currentHp += shield;
        shield = 0.0f;
    }

    if (currentHp <= 0)
    {
        if (secondLife)
        {
            secondLife = false;
            currentHp = 1;
            inmunityTime = 2.0f;
            Audio::Event("starlord_damaged"); // second life audio
            // hit animation?
        }
        else
        {
            currentHp = 0;
            Scene::LoadScene("LoseMenu.HScene");
            Audio::Event("starlord_dead");
            // death
        }
    }
    else
    {
        inmunityTime = 2.0f;
        Audio::Event("starlord_damaged");
        // hit animation?
    }

    lastHitTime = 3.0f; // 3 seg to auto heal after a hit
}

void PlayerStats::Heal(float amount)
{
    currentHp += amount;

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
        return fireAmmo;
        break;
    case 3:
        return ricochetAmmo;
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
        fireAmmo += amount;
        if (fireAmmo > maxFireAmmo) fireAmmo = maxFireAmmo;
        break;
    case 3:
        ricochetAmmo += amount;
        if (ricochetAmmo > maxRicochetAmmo) ricochetAmmo = maxRicochetAmmo;
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
        fireAmmo -= amount;
        break;
    case 3:
        ricochetAmmo -= amount;
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
    switch (index)
    {
    case 0:
        speedPowerUp = 5;
        break;
    case 1:
        fireratePowerUp = 5;
        break;
    case 2:
        shield = 50;
        break;
    case 3:
        GetAmmo(1, 9999);
        GetAmmo(2, 9999);
        GetAmmo(3, 9999);
        break;
    case 4:
        slowTimePowerUp = 5.0f;
        Time::ChangeTimeScale(0.5f);
        break;
    default:
        Console::Log("Invalid powe up index, can only be 0, 1, 2 or 3.");
        break;
    }
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
        storage->unlockGunBlueprint = index;
        break;
    default:
        break;
    }

    storage->SaveData();
}