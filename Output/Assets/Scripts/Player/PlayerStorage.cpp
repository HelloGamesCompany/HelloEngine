#include "PlayerStorage.h"
#include "API/API_QuickSave.h"
HELLO_ENGINE_API_C PlayerStorage* CreatePlayerStorage(ScriptToInspectorInterface* script)
{
    PlayerStorage* classInstance = new PlayerStorage();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragInt("Level Index", &classInstance->levelIndex);
    return classInstance;
}

void PlayerStorage::Start()
{
    unlockGunBlueprint = -1; // -1 --> no gun unlocked
    upgradeBlueprintAmount = API_QuickSave::GetInt("upgradeBlueprintAmount");

    switch (levelIndex)
    {
    case 0: // hub
        // no casettes in hub
        break;
    case 1: // level 1
        casette1Picked = API_QuickSave::GetBool("level1_casette1");
        casette2Picked = API_QuickSave::GetBool("level1_casette2");
        casette3Picked = API_QuickSave::GetBool("level1_casette3");
        break;
    case 2: // level 2
        casette1Picked = API_QuickSave::GetBool("level2_casette1");
        casette2Picked = API_QuickSave::GetBool("level2_casette2");
        casette3Picked = API_QuickSave::GetBool("level2_casette3");
        break;
    case 3: // level 3
        casette1Picked = API_QuickSave::GetBool("level3_casette1");
        casette2Picked = API_QuickSave::GetBool("level3_casette2");
        casette3Picked = API_QuickSave::GetBool("level3_casette3");
        break;
    case 4: // level 4
        casette1Picked = API_QuickSave::GetBool("level4_casette1");
        casette2Picked = API_QuickSave::GetBool("level4_casette2");
        casette3Picked = API_QuickSave::GetBool("level4_casette3");
        break;
    case 5: // level 5
        casette1Picked = API_QuickSave::GetBool("level5_casette1");
        casette2Picked = API_QuickSave::GetBool("level5_casette2");
        casette3Picked = API_QuickSave::GetBool("level5_casette3");
        break;
    default:
        Console::Log("Wrong level index", API::Console::MessageType::ERR);
        break;
    }
}

void PlayerStorage::Update()
{

}

void PlayerStorage::SaveData()
{
    // save unlocked gun
    switch (unlockGunBlueprint)
    {
    case 1:
        API_QuickSave::SetBool("semiautomaticGunUnlocked", true);
        break;
    case 2:
        API_QuickSave::SetBool("automaticGunUnlocked", true);
        break;
    case 3:
        API_QuickSave::SetBool("burstGunUnlocked", true);
        break;
    case 4:
        API_QuickSave::SetBool("shotgunGunUnlocked", true);
        break;
    case 5:
        API_QuickSave::SetBool("handgunGunUnlocked", true);
        break;
    default:
        break;
    }

    // save upgradeBlueprint amount
    API_QuickSave::SetInt("upgradeBlueprintAmount", upgradeBlueprintAmount);

    // save casettes
    switch (levelIndex)
    {
    case 0:
        break;
    case 1:
        API_QuickSave::SetBool("level1_casette1", casette1Picked);
        API_QuickSave::SetBool("level1_casette2", casette2Picked);
        API_QuickSave::SetBool("level1_casette3", casette3Picked);
        break;
    case 2:
        API_QuickSave::SetBool("level2_casette1", casette1Picked);
        API_QuickSave::SetBool("level2_casette2", casette2Picked);
        API_QuickSave::SetBool("level2_casette3", casette3Picked);
        break;
    case 3:
        API_QuickSave::SetBool("level3_casette1", casette1Picked);
        API_QuickSave::SetBool("level3_casette2", casette2Picked);
        API_QuickSave::SetBool("level3_casette3", casette3Picked);
        break;
    case 4:
        API_QuickSave::SetBool("level4_casette1", casette1Picked);
        API_QuickSave::SetBool("level4_casette2", casette2Picked);
        API_QuickSave::SetBool("level4_casette3", casette3Picked);
        break;
    case 5:
        API_QuickSave::SetBool("level5_casette1", casette1Picked);
        API_QuickSave::SetBool("level5_casette2", casette2Picked);
        API_QuickSave::SetBool("level5_casette3", casette3Picked);
        break;
    default:
        Console::Log("Wrong level index", API::Console::MessageType::ERR);
        break;
    }
}

void PlayerStorage::SaveDataFromChest(int chestIndex, int chestContent)
{
    if (levelIndex == 0) return;

    std::string saveActiveLabel = "level" + std::to_string(levelIndex) + "_chest" + std::to_string(chestIndex + 1);
    std::string saveContentLabel = saveActiveLabel + "_content";

    API_QuickSave::SetBool(saveActiveLabel, false); // active == false

    if (chestContent <= 0) API_QuickSave::SetInt(saveContentLabel, 2); // chest have upgradeBlueprint
    else if (chestContent <= 5) API_QuickSave::SetInt(saveContentLabel, 1); // chest have gunBlueprint
    else API_QuickSave::SetInt(saveContentLabel, 0); // chest have specialGun
}