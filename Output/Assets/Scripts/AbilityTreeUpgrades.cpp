#include "AbilityTreeUpgrades.h"
HELLO_ENGINE_API_C AbilityTreeUpgrades* CreateAbilityTreeUpgrades(ScriptToInspectorInterface* script)
{
    AbilityTreeUpgrades* classInstance = new AbilityTreeUpgrades();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxUIButton("Upgrate 1", &classInstance->Upgrate1);
    script->AddDragInt("Skillpoints needed LvL 1", &classInstance->skillPoints1);
    script->AddDragBoxUIButton("Upgrate 2", &classInstance->Upgrate2);
    script->AddDragInt("Skillpoints needed LvL 2", &classInstance->skillPoints2);
    script->AddDragBoxUIButton("Upgrate 3", &classInstance->Upgrate3);
    script->AddDragInt("Skillpoints needed LvL 3", &classInstance->skillPoints3);
    script->AddDragBoxUIButton("Upgrate 4", &classInstance->Upgrate4);
    script->AddDragInt("Skillpoints needed LvL 4", &classInstance->skillPoints4);
    script->AddDragBoxUIButton("Upgrate 5", &classInstance->Upgrate5);
    script->AddDragInt("Skillpoints needed LvL 5", &classInstance->skillPoints5);
    script->AddDragInt("Upgrade Number", &classInstance->upgradeNum);
    script->AddDragBoxUIInput("Main Panel", &classInstance->mainPanel);
    script->AddDragBoxUIInput("Current Panel", &classInstance->currentPanel);
    script->AddDragInt("Tree Index", &classInstance->treeIndex);
    script->AddDragBoxGameObject("Player", &classInstance->playerStorageGO);
    script->AddDragBoxGameObject("Description 1", &classInstance->description1);
    script->AddDragBoxGameObject("Description 2", &classInstance->description2);
    script->AddDragBoxGameObject("Description 3", &classInstance->description3);
    script->AddDragBoxGameObject("Description 4", &classInstance->description4);
    script->AddDragBoxGameObject("Description 5", &classInstance->description5);
    return classInstance;
}

void AbilityTreeUpgrades::Start()
{
    skillLevel = API_QuickSave::GetInt("tree" + std::to_string(treeIndex) + "_level");

    playerStorage = (PlayerStorage*)playerStorageGO.GetScript("PlayerStorage");
    if (playerStorage == nullptr) Console::Log("Player Storage missing in AbilitytreeUpdates.");
    currentPanel.SetEnable(false);

    if (skillLevel != 0) Upgrate1.SetBlocked(true);
    if (skillLevel != 1) Upgrate2.SetBlocked(true);
    if (skillLevel != 2) Upgrate3.SetBlocked(true);
    if (skillLevel != 3 && upgradeNum > 3) Upgrate4.SetBlocked(true);
    if (skillLevel != 4 && upgradeNum > 4) Upgrate5.SetBlocked(true);
    description2.SetActive(false);
    description3.SetActive(false);
    description4.SetActive(false);
    description5.SetActive(false);
}

void AbilityTreeUpgrades::Update()
{
    if (Input::GetGamePadButton(GamePadButton::BUTTON_B) == KeyState::KEY_DOWN && currentPanel.IsEnabled())
    {
        Input::HandleGamePadButton(GamePadButton::BUTTON_B);;
        mainPanel.SetEnable(true);
        currentPanel.SetEnable(false);
        return;
    }

    if (!playerStorage) return;

    if (manteinTime > 0.0f)
    {
        manteinTime -= Time::GetRealTimeDeltaTime();

        if (Upgrate1.OnRelease() || Upgrate2.OnRelease() || Upgrate3.OnRelease() || Upgrate4.OnRelease() || Upgrate5.OnRelease())
        {
            manteinTime = 0.0f;
        }
        else if (manteinTime <= 0.0f)
        {
            manteinTime = 0.0f;

            if (upgradingLevel == 1)
            {
                UpgradeSkill();
                Upgrate1.SetBlocked(true);
                Upgrate2.SetBlocked(false);
                description1.SetActive(true);
                description2.SetActive(false);
            }
            else if (upgradingLevel == 2)
            {
                UpgradeSkill();
                Upgrate2.SetBlocked(true);
                Upgrate3.SetBlocked(false);
                description1.SetActive(false);
                description2.SetActive(true);
                description3.SetActive(false);
            }
            else if (upgradingLevel == 3)
            {
                UpgradeSkill();
                Upgrate3.SetBlocked(true);
                description2.SetActive(false);
                description3.SetActive(true);
                description4.SetActive(false);
                if (upgradeNum > 3) Upgrate4.SetBlocked(false);
            }
            else if (upgradingLevel == 4)
            {
                UpgradeSkill();
                Upgrate4.SetBlocked(true);
                if (upgradeNum > 4) Upgrate5.SetBlocked(false);
                description3.SetActive(false);
                description4.SetActive(true);
                description5.SetActive(false);
            }
            else if (upgradingLevel == 5)
            {
                UpgradeSkill();
                Upgrate5.SetBlocked(true);
                description4.SetActive(false);
                description5.SetActive(true);
            }
        }
    }

    if (Upgrate1.OnPress() && manteinTime == 0.0f)
    {
        if (playerStorage->skillPoints > skillPoints1)
        {
            manteinTime = 1.0f;
            upgradingLevel = 1;
            upgradeCost = skillPoints1;
        }
        else
        {
            // wrong sound
        }
    }

    if (Upgrate2.OnPress() && manteinTime == 0.0f)
    {
        if (playerStorage->skillPoints > skillPoints2)
        {
            manteinTime = 1.0f;
            upgradingLevel = 2;
            upgradeCost = skillPoints2;
        }
        else
        {
            // wrong sound
        }
    }

    if (Upgrate3.OnPress() && manteinTime == 0.0f)
    {
        if (playerStorage->skillPoints > skillPoints3)
        {
            manteinTime = 1.0f;
            upgradingLevel = 3;
            upgradeCost = skillPoints3;
        }
        else
        {
            // wrong sound
        }
    }

    if (Upgrate4.OnPress() && manteinTime == 0.0f)
    {
        if (playerStorage->skillPoints > skillPoints4)
        {
            manteinTime = 1.0f;
            upgradingLevel = 4;
            upgradeCost = skillPoints4;
        }
        else
        {
            // wrong sound
        }
    }

    if (Upgrate5.OnPress() && manteinTime == 0.0f)
    {
        if (playerStorage->skillPoints > skillPoints5)
        {
            manteinTime = 1.0f;
            upgradingLevel = 5;
            upgradeCost = skillPoints5;
        }
        else
        {
            // wrong sound
        }
    }
}

void AbilityTreeUpgrades::UpgradeSkill()
{
    playerStorage->skillPoints -= upgradeCost;
    playerStorage->SaveData();

    skillLevel++;
    API_QuickSave::SetInt("tree" + std::to_string(treeIndex) + "_level", skillLevel);
}