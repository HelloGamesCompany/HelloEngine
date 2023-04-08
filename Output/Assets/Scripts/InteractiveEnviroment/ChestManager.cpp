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
    check = false;
}

void ChestManager::Update()
{
    if (check) return;

    Chest* chestScript = (Chest*)chest1.GetScript("Chest");
    if (chestScript->tutorialSpecialWeapon) chestScript->itemIndex = GetSpecialGun();
    else if (chestScript->tutorialWeaponBlueprint) chestScript->itemIndex = GetGunBlueprint();
    else
    {

    }

    check = true;
}

int ChestManager::GetGunBlueprint()
{
    float random = rand() % 5;
    return random + 1;
}

int ChestManager::GetSpecialGun()
{
    float random = rand() % 2;
    return random + 6;
}