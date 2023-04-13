#include "Level2EndTrigger.h"

#include "Player/PlayerStorage.h"

HELLO_ENGINE_API_C Level2EndTrigger* CreateLevel2EndTrigger(ScriptToInspectorInterface* script)
{
	Level2EndTrigger* classInstance = new Level2EndTrigger();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxGameObject("Player Storage GO", &classInstance->playerStorageGO);
	script->AddDragBoxGameObject("Final text panel: ", &classInstance->finalTextPanel);
	return classInstance;
}

void Level2EndTrigger::Start()
{
	playerStorage = (PlayerStorage*)playerStorageGO.GetScript("PlayerStorage");
	if (playerStorage == nullptr) Console::Log("PlayerStorage missing in GetDiviner Script.");
}
void Level2EndTrigger::Update()
{

}

void Level2EndTrigger::OnCollisionEnter(API_RigidBody other)
{
    std::string detectionName = other.GetGameObject().GetName();
    if (detectionName == "Player")
    {
        if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_DOWN || Input::GetKey(KeyCode::KEY_E) == KeyState::KEY_DOWN)
        {
            if (playerStorage)
            {
                playerStorage->skillPoints += 5;
                playerStorage->SaveData();
            }
            API_QuickSave::SetBool("level2_completed", true);
            finalTextPanel.SetActive(true);
        }

    }
}
