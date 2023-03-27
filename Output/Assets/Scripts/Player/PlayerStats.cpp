#include "PlayerStats.h"
#include "../UI Test folder/HpBar.h"
HELLO_ENGINE_API_C PlayerStats* CreatePlayerStats(ScriptToInspectorInterface* script)
{
    PlayerStats* classInstance = new PlayerStats();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragFloat("Max HP", &classInstance->maxHp);
    script->AddDragFloat("Upgraded Max HP", &classInstance->upgradedMaxHp);
    script->AddDragInt("Laser Ammo", &classInstance->laserAmmo);
    script->AddDragInt("Fire Ammo", &classInstance->fireAmmo);
    script->AddDragInt("Ricochet Ammo", &classInstance->ricochetAmmo);
    //script->AddDragBoxGameObject("Health bar", &classInstance->hpGameObject);
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

    shield = 0;
}

void PlayerStats::Update()
{
    //healthBar->hp = this->currentHp;

    // deadline healing
    float deathlineHp;
    if (healthTreeLvl > 1) deathlineHp = currentMaxHp * 0.25f;
    else deathlineHp = currentMaxHp * 0.20f;
    if (currentHp < deathlineHp)
    {
        lastHitTime -= Time::GetDeltaTime();
        if (lastHitTime <= 0.0f)
        {
            if (healthTreeLvl > 3) currentHp += 8.0f;
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
        inmunityTime -= Time::GetDeltaTime();
    }
}

void PlayerStats::TakeDamage(float amount)
{
    if (inmunityTime > 0.0f) return; // only VS2

    shield -= amount;
    if (shield <= 0.0f) currentHp += shield;


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
    default:
        Console::Log("Invalid powe up index, can only be 0, 1, 2 or 3.");
        break;
    }
}