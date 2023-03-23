#include "ChestTest.h"
HELLO_ENGINE_API_C ChestTest* CreateChestTest(ScriptToInspectorInterface* script)
{
    ChestTest* classInstance = new ChestTest();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragInt("Gun Index", &classInstance->gunInsideChest);
    script->AddDragInt("Gun Stot", &classInstance->gunInsideChestSlot);
    return classInstance;
}

void ChestTest::Start()
{
    
}

void ChestTest::Update()
{
    
}

void ChestTest::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionName = other.GetGameObject().GetName();
    if (detectionName == "Player")
    {
        if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_DOWN || Input::GetKey(KeyCode::KEY_E) == KeyState::KEY_DOWN)
        {
            PlayerGunManager* playerGunManager = (PlayerGunManager*)other.GetGameObject().GetScript("PlayerGunManager");
            playerGunManager->GetGun(gunInsideChestSlot, gunInsideChest);
            gameObject.SetActive(false);
        }
    }
}