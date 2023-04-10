#include "DisplayUpgradeBpAmount.h"
#include "../../Player/PlayerStorage.h"
HELLO_ENGINE_API_C DisplayUpgradeBpAmount* CreateDisplayUpgradeBpAmount(ScriptToInspectorInterface* script)
{
    DisplayUpgradeBpAmount* classInstance = new DisplayUpgradeBpAmount();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxUIText("Text Blueprints", &classInstance->bluePrintAmountText);
    script->AddDragBoxGameObject("PlayerStorage GO", &classInstance->playerStorageGO);
    return classInstance;
}

void DisplayUpgradeBpAmount::Start()
{
    playerStorage = (PlayerStorage*)playerStorageGO.GetScript("PlayerStorage");
    if (playerStorage == nullptr) Console::Log("Player Storage missing in DisplayUpgradeBpAmount.");
}

void DisplayUpgradeBpAmount::Update()
{
    if (playerStorage) bluePrintAmountText.SetText(std::to_string(playerStorage->upgradeBlueprintAmount).c_str());
}