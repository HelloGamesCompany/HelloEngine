#include "DisplaySkillPointsAmount.h"
#include "../../Player/PlayerStorage.h"
HELLO_ENGINE_API_C DisplaySkillPointsAmount* CreateDisplaySkillPointsAmount(ScriptToInspectorInterface* script)
{
    DisplaySkillPointsAmount* classInstance = new DisplaySkillPointsAmount();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxUIText("Text Blueprints", &classInstance->skillPointsAmountText);
    script->AddDragBoxGameObject("PlayerStorage GO", &classInstance->playerStorageGO);
    return classInstance;
}

void DisplaySkillPointsAmount::Start()
{
    playerStorage = (PlayerStorage*)playerStorageGO.GetScript("PlayerStorage");
    if (playerStorage == nullptr) Console::Log("Player Storage missing in DisplaySkillPointsAmount.");
}

void DisplaySkillPointsAmount::Update()
{
    if (playerStorage) skillPointsAmountText.SetText(std::to_string(playerStorage->skillPoints).c_str());
}