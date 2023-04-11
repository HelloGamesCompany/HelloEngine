#include "AbilityTreeUpgrades.h"
HELLO_ENGINE_API_C AbilityTreeUpgrades* CreateAbilityTreeUpgrades(ScriptToInspectorInterface* script)
{
    AbilityTreeUpgrades* classInstance = new AbilityTreeUpgrades();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxUIButton("Ability1", &classInstance->buttonHovered);
    script->AddDragBoxUIInput("Main Panel", &classInstance->mainPanel);
    script->AddDragBoxUIInput("Current Panel", &classInstance->currentPanel);
    script->AddDragInt("Upgrade Index", &classInstance->upgradeIndex);
    script->AddDragInt("Tree Index", &classInstance->treeIndex);
    script->AddDragBoxGameObject("Player", &classInstance->playerStorageGO);
    return classInstance;
}

void AbilityTreeUpgrades::Start()
{
    skillLevel = API_QuickSave::GetInt("tree" + std::to_string(treeIndex) + "_level");

    if (skillLevel + 1 != upgradeIndex) buttonHovered.SetBlocked(true);

    playerStorage = (PlayerStorage*)playerStorageGO.GetScript("PlayerStorage");
    if (playerStorage == nullptr) Console::Log("Player Storage missing in AbilitytreeUpdates.");
}

void AbilityTreeUpgrades::Update()
{
    if (Input::GetGamePadButton(GamePadButton::BUTTON_B) == KeyState::KEY_DOWN && currentPanel.IsEnabled() && upgradeIndex == 1)
    {
        mainPanel.SetEnable(true);
        currentPanel.SetEnable(false);
    }
    
    skillLevel = API_QuickSave::GetInt("tree" + std::to_string(treeIndex) + "_level");
    if (skillLevel + 1 != upgradeIndex) buttonHovered.SetBlocked(true);
    else buttonHovered.SetBlocked(false);

    if (!playerStorage) return;

    if (manteinTime > 0.0f)
    {
        manteinTime -= Time::GetRealTimeDeltaTime();

        if (buttonHovered.OnRelease())
        {
            manteinTime = 0.0f;
        }
        else if (manteinTime <= 0.0f)
        {
            manteinTime = 0.0f;

            playerStorage->skillPoints--;
            playerStorage->SaveData();

            skillLevel++;
            API_QuickSave::SetInt("tree" + std::to_string(treeIndex) + "_level", skillLevel);

            buttonHovered.SetBlocked(true);
        }
    }

    if (buttonHovered.OnPress() && manteinTime == 0.0f && playerStorage->skillPoints > 0)
    {
        manteinTime = 1.0f;
    }
}