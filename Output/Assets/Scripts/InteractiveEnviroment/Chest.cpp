#include "Chest.h"
HELLO_ENGINE_API_C Chest* CreateChest(ScriptToInspectorInterface* script)
{
    Chest* classInstance = new Chest();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxAnimationPlayer("Chest Animator Player", &classInstance->chestAnimatorPlayer);
    script->AddDragFloat("Open Chest Time", &classInstance->maxOpenChestTime);
    script->AddDragInt("Item Index", &classInstance->itemIndex);
    return classInstance;
}

void Chest::Start()
{
    openChestTime = maxOpenChestTime;
    opening = false;
}

void Chest::Update()
{
    if (opening)
    {
        openChestTime -= Time::GetRealTimeDeltaTime();

        if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_UP || Input::GetKey(KeyCode::KEY_E) == KeyState::KEY_UP)
        {
            playerMove->StopOpenChestAnim();
            openChestTime = maxOpenChestTime;
            opening = false;
        }
        if (openChestTime <= 0.0f)
        {
            opening = false;

            if (!playerGunManager || !playerStats || !playerMove)
            {
                openChestTime = maxOpenChestTime;
                return;
            }

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
                playerStats->SaveInStorage(-1); // save game
                break;
            case 7: // Get Ricochet
                playerGunManager->GetGun(3, 7);
                playerStats->GetAmmo(3, 15);
                playerStats->SaveInStorage(-1); // save game
                break;
            default:
                Console::Log("Item Index is not between 0 and 7.");
                break;
            }

            playerMove->StopOpenChestAnim();
            chestAnimatorPlayer.Play();
            gameObject.SetActive(false);
        }
    }
}

void Chest::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player")
    {
        if (opening) return;

        if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_DOWN || Input::GetKey(KeyCode::KEY_E) == KeyState::KEY_DOWN)
        {
            playerGunManager = (PlayerGunManager*)other.GetGameObject().GetScript("PlayerGunManager");
            playerStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
            playerMove = (PlayerMove*)other.GetGameObject().GetScript("PlayerMove");

            if (playerMove) playerMove->PlayOpenChestAnim();
            opening = true;
        }
    }
}