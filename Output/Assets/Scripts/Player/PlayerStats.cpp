#include "PlayerStats.h"
#include "../UI Test folder/HpBar.h"
HELLO_ENGINE_API_C PlayerStats* CreatePlayerStats(ScriptToInspectorInterface* script)
{
    PlayerStats* classInstance = new PlayerStats();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragFloat("Max HP", &classInstance->maxHp);
    script->AddDragInt("Laser Ammo", &classInstance->laserAmmo);
    script->AddDragInt("Fire Ammo", &classInstance->fireAmmo);
    script->AddDragInt("Ricochet Ammo", &classInstance->ricochetAmmo);
    script->AddDragBoxGameObject("Health bar", &classInstance->hpGameObject);
    return classInstance;
}

void PlayerStats::Start()
{
    currentHp = maxHp;
    healthBar = (HpBar*)hpGameObject.GetScript("HpBar");
    detected = false;
}

void PlayerStats::Update()
{
    healthBar->maxHp = this->maxHp;
    healthBar->hp = this->currentHp;
    // test purpose to show on VS2
    if (Input::GetKey(KeyCode::KEY_Y) == KeyState::KEY_DOWN) TakeDamage(5.0f);
    if (Input::GetKey(KeyCode::KEY_U) == KeyState::KEY_DOWN) TakeDamage(maxHp / 2.0f); // heal only testing
    if (inmunityTime > 0.0f)
    {
        inmunityTime -= Time::GetDeltaTime();
    }
}

void PlayerStats::TakeDamage(float amount)
{
    if (inmunityTime > 0.0f) return; // only VS2

    currentHp -= amount;
    if (currentHp <= 0)
    {
        currentHp = 0;
        // death
    }
    else
    {
        inmunityTime = 2.0f;
        // hit animation?
    }
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