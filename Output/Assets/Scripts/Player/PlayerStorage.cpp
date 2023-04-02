#include "PlayerStorage.h"
HELLO_ENGINE_API_C PlayerStorage* CreatePlayerStorage(ScriptToInspectorInterface* script)
{
    PlayerStorage* classInstance = new PlayerStorage();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    return classInstance;
}

void PlayerStorage::Start()
{
    int unlockGunBlueprint = -1; // -1 --> no gun unlocked
    int upgradeBlueprintAmount = 0;

    casette1Picked = false; // loaded from save data
    casette2Picked = false;
    casette3Picked = false;
}

void PlayerStorage::Update()
{

}