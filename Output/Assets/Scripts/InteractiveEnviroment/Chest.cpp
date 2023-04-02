#include "Chest.h"
#include "../Shooting/PlayerGunManager.h"
#include "../Player/PlayerStats.h"
HELLO_ENGINE_API_C Chest* CreateChest(ScriptToInspectorInterface* script)
{
    Chest* classInstance = new Chest();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragInt("Item Index", &classInstance->itemIndex);
    return classInstance;
}

void Chest::Start()
{

}

void Chest::Update()
{

}

void Chest::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player")
    {
        if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_DOWN || Input::GetKey(KeyCode::KEY_E) == KeyState::KEY_DOWN)
        {
            PlayerGunManager* playerGunManager = (PlayerGunManager*)other.GetGameObject().GetScript("PlayerGunManager");
            PlayerStats* playerStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");

            switch (itemIndex)
            {
            case 0: // Upgrade Blueprint
            case 1: // Unlock Gun
            case 2:
            case 3:
            case 4:
            case 5:
                playerStats->SaveInStorage(itemIndex);
                break;
            case 6: // Get Flamethrower
                playerGunManager->GetGun(3, 6);
                playerStats->GetAmmo(2, 200);
                break;
            case 7: // Get Ricochet
                playerGunManager->GetGun(3, 7);
                playerStats->GetAmmo(3, 15);
                break;
            default:
                Console::Log("Item Index is not between 0 and 7.");
                break;
            }
            
            gameObject.SetActive(false);
        }
    }
}