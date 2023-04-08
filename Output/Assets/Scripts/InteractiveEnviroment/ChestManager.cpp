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
    script->AddDragBoxGameObject("Player Storage GO", &classInstance->playerStorageGO);
    return classInstance;
}

void ChestManager::Start()
{
    playerStorage = (PlayerStorage*)playerStorageGO.GetScript("PlayerStorage");
    if (playerStorage == nullptr) Console::Log("PlayerStorage missing in CasetteManager.");

    specialGunAsigned = false;
    gunBlueprintAsigned = false;
    remainingUpgradeBlueprints = 2;

    check = false;
}

void ChestManager::Update()
{
    if (check || !playerStorage) return;

    std::string saveActiveLabel = "level" + std::to_string(playerStorage->levelIndex) + "_chest";

    Chest* chestScript = (Chest*)chest1.GetScript("Chest");
    if (chestScript)
    {
        if (API_QuickSave::GetBool(saveActiveLabel + "1")) // if chest is not active on save
        {
            int loot = API_QuickSave::GetInt(saveActiveLabel + "1" + "_content");

            switch (loot)
            {
            case 0:
                specialGunAsigned = true;
                break;
            case 1:
                gunBlueprintAsigned = true;
                break;
            case 2:
                remainingUpgradeBlueprints--;
                break;
            default:
                Console::Log("Chest save is wrong.", API::Console::MessageType::ERR);
                break;
            }

            chestScript->OpenChestOnStart();
        }
        else
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
            chestScript->chestIndex = 0;
        }
    }

    chestScript = (Chest*)chest2.GetScript("Chest");
    if (chestScript)
    {
        if (API_QuickSave::GetBool(saveActiveLabel + "2")) // if chest is not active on save
        {
            int loot = API_QuickSave::GetInt(saveActiveLabel + "2" + "_content");

            switch (loot)
            {
            case 0:
                specialGunAsigned = true;
                break;
            case 1:
                gunBlueprintAsigned = true;
                break;
            case 2:
                remainingUpgradeBlueprints--;
                break;
            default:
                Console::Log("Chest save is wrong.", API::Console::MessageType::ERR);
                break;
            }

            chestScript->OpenChestOnStart();
        }
        else
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
            chestScript->chestIndex = 1;
        }
    }

    chestScript = (Chest*)chest3.GetScript("Chest");
    if (chestScript)
    {
        if (API_QuickSave::GetBool(saveActiveLabel + "3")) // if chest is not active on save
        {
            int loot = API_QuickSave::GetInt(saveActiveLabel + "3" + "_content");

            switch (loot)
            {
            case 0:
                specialGunAsigned = true;
                break;
            case 1:
                gunBlueprintAsigned = true;
                break;
            case 2:
                remainingUpgradeBlueprints--;
                break;
            default:
                Console::Log("Chest save is wrong.", API::Console::MessageType::ERR);
                break;
            }

            chestScript->OpenChestOnStart();
        }
        else
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
            chestScript->chestIndex = 2;
        }
    }

    chestScript = (Chest*)chest4.GetScript("Chest");
    if (chestScript)
    {
        if (API_QuickSave::GetBool(saveActiveLabel + "4")) // if chest is not active on save
        {
            int loot = API_QuickSave::GetInt(saveActiveLabel + "4" + "_content");

            switch (loot)
            {
            case 0:
                specialGunAsigned = true;
                break;
            case 1:
                gunBlueprintAsigned = true;
                break;
            case 2:
                remainingUpgradeBlueprints--;
                break;
            default:
                Console::Log("Chest save is wrong.", API::Console::MessageType::ERR);
                break;
            }

            chestScript->OpenChestOnStart();
        }
        else
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
            chestScript->chestIndex = 3;
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