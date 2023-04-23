#include "FindAndRescueGroot.h"
#include "../Player/PlayerStorage.h"

HELLO_ENGINE_API_C FindAndRescueGroot* CreateFindAndRescueGroot(ScriptToInspectorInterface* script)
{
	FindAndRescueGroot* classInstance = new FindAndRescueGroot();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxGameObject("Player Storage GO", &classInstance->playerStorageGO);
	return classInstance;
}

void FindAndRescueGroot::Start()
{
    playerStorage = (PlayerStorage*)playerStorageGO.GetScript("PlayerStorage");
    if (playerStorage == nullptr) Console::Log("PlayerStorage missing in FindAndRescue Script.");
}
void FindAndRescueGroot::Update()
{

}

void FindAndRescueGroot::OnCollisionStay(API_RigidBody other)
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
 
        }

    }
}