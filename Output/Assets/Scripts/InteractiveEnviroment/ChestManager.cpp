#include "ChestManager.h"
#include "Chest.h"
HELLO_ENGINE_API_C ChestManager* CreateChestManager(ScriptToInspectorInterface* script)
{
    ChestManager* classInstance = new ChestManager();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Chest 1", &classInstance->chest1);
    script->AddDragBoxGameObject("Chest 2", &classInstance->chest2);
    script->AddDragBoxGameObject("Chest 3", &classInstance->chest3);
    script->AddDragBoxGameObject("Chest 4", &classInstance->chest4);
    return classInstance;
}

void ChestManager::Start()
{
    specialGunAsigned = false;
    gunBlueprintAsigned = false;
    remainingUpgradeBlueprints = 2;

    check = false;
}

void ChestManager::Update()
{
    if (check) return;

    Chest* chestScript = (Chest*)chest1.GetScript("Chest");
    if (chestScript)
    {
        if (chestScript->tutorialSpecialWeapon) chestScript->itemIndex = GetSpecialGun();
        else if (chestScript->tutorialWeaponBlueprint) chestScript->itemIndex = GetGunBlueprint();
        else
        {
            int loot = GetRandomLoot();
            
            if (loot == 0)
            {
                chestScript->itemIndex = GetSpecialGun();
            }
            else if (loot == 1)
            {
                chestScript->itemIndex = GetGunBlueprint();
            }
            else // upgrade blueprint
            {
                chestScript->itemIndex = 0;
                remainingUpgradeBlueprints--;
            }
        }
    }

    chestScript = (Chest*)chest2.GetScript("Chest");
    if (chestScript)
    {
        if (chestScript->tutorialSpecialWeapon) chestScript->itemIndex = GetSpecialGun();
        else if (chestScript->tutorialWeaponBlueprint) chestScript->itemIndex = GetGunBlueprint();
        else
        {
            int loot = GetRandomLoot();
            
            if (loot == 0)
            {
                chestScript->itemIndex = GetSpecialGun();
            }
            else if (loot == 1)
            {
                chestScript->itemIndex = GetGunBlueprint();
            }
            else // upgrade blueprint
            {
                chestScript->itemIndex = 0;
                remainingUpgradeBlueprints--;
            }
        }
    }

    chestScript = (Chest*)chest3.GetScript("Chest");
    if (chestScript)
    {
        if (chestScript->tutorialSpecialWeapon) chestScript->itemIndex = GetSpecialGun();
        else if (chestScript->tutorialWeaponBlueprint) chestScript->itemIndex = GetGunBlueprint();
        else
        {
            int loot = GetRandomLoot();
            
            if (loot == 0)
            {
                chestScript->itemIndex = GetSpecialGun();
            }
            else if (loot == 1)
            {
                chestScript->itemIndex = GetGunBlueprint();
            }
            else // upgrade blueprint
            {
                chestScript->itemIndex = 0;
                remainingUpgradeBlueprints--;
            }
        }
    }

    chestScript = (Chest*)chest4.GetScript("Chest");
    if (chestScript)
    {
        if (chestScript->tutorialSpecialWeapon) chestScript->itemIndex = GetSpecialGun();
        else if (chestScript->tutorialWeaponBlueprint) chestScript->itemIndex = GetGunBlueprint();
        else
        {
            int loot = GetRandomLoot();
            
            if (loot == 0)
            {
                chestScript->itemIndex = GetSpecialGun();
            }
            else if (loot == 1)
            {
                chestScript->itemIndex = GetGunBlueprint();
            }
            else // upgrade blueprint
            {
                chestScript->itemIndex = 0;
                remainingUpgradeBlueprints--;
            }
        }
    }

    check = true;
}

int ChestManager::GetRandomLoot()
{
    if (specialGunAsigned && gunBlueprintAsigned && remainingUpgradeBlueprints == 0) return -1;

    int random = rand() % 4;

    if (random == 0)
    {
        if (!specialGunAsigned) return 0;
        else return GetRandomLoot();
    }
    else if (random == 1)
    {
        if (!gunBlueprintAsigned) return 1;
        else return GetRandomLoot();
    }
    else if (random > 1)
    {
        if (remainingUpgradeBlueprints > 0) return 2;
        else return GetRandomLoot();
    }
}

int ChestManager::GetGunBlueprint()
{
    gunBlueprintAsigned = true;
    int random = rand() % 5;
    return random + 1;
}

int ChestManager::GetSpecialGun()
{
    specialGunAsigned = true;
    int random = rand() % 2;
    return random + 6;
}