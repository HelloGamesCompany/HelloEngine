#include "Chest.h"
HELLO_ENGINE_API_C Chest* CreateChest(ScriptToInspectorInterface* script)
{
    Chest* classInstance = new Chest();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxAnimationPlayer("Chest Animator Player", &classInstance->chestAnimatorPlayer);
    script->AddDragFloat("Open Chest Time", &classInstance->maxOpenChestTime);
    script->AddCheckBox("Tutorial Special Weapon", &classInstance->tutorialSpecialWeapon);
    script->AddCheckBox("Tutorial Weapon Blueprint", &classInstance->tutorialWeaponBlueprint);
    script->AddDragInt("Chest Index", &classInstance->chestIndex);
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
            if (playerMove) playerMove->StopOpenChestAnim();
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
                playerStats->SaveChestData(itemIndex, chestIndex);
                break;
            case 6: // Get Flamethrower
                playerGunManager->GetGun(3, 6);
                playerStats->specialAmmo = 200;
                playerStats->GetAmmo(2, 200);
                playerStats->SaveChestData(6, chestIndex); // save game
                break;
            case 7: // Get Ricochet
                playerGunManager->GetGun(3, 7);
                playerStats->specialAmmo = 15;
                playerStats->GetAmmo(3, 15);
                playerStats->SaveChestData(7, chestIndex); // save game
                break;
            default:
                Console::Log("Item Index is not between 0 and 7.");
                break;
            }
            
            Audio::Event("open_chest");
            playerMove->StopOpenChestAnim();
            chestAnimatorPlayer.Play();
            gameObject.SetActive(false);
        }
    }
}

void Chest::OnCollisionStay(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player")
    {
        if (opening) return;

        if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_DOWN || Input::GetKey(KeyCode::KEY_E) == KeyState::KEY_DOWN)
        {
            playerMove = (PlayerMove*)other.GetGameObject().GetScript("PlayerMove");
            if (playerMove == nullptr) return;

            float distanceX = gameObject.GetTransform().GetGlobalPosition().x - other.GetGameObject().GetTransform().GetGlobalPosition().x;
            float distanceZ = gameObject.GetTransform().GetGlobalPosition().z - other.GetGameObject().GetTransform().GetGlobalPosition().z;

            if (abs(distanceX) < abs(distanceZ))
            {
                if (distanceZ >= 0.0f && playerMove->aimAngle <= 90 && playerMove->aimAngle > -90) // chest up
                {
                    playerGunManager = (PlayerGunManager*)other.GetGameObject().GetScript("PlayerGunManager");
                    playerStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");

                    if (playerMove) playerMove->PlayOpenChestAnim();
                    opening = true;
                }
                else if (distanceZ < 0.0f && playerMove->aimAngle <= -90 && playerMove->aimAngle > -270) // chest down
                {
                    playerGunManager = (PlayerGunManager*)other.GetGameObject().GetScript("PlayerGunManager");
                    playerStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");

                    if (playerMove) playerMove->PlayOpenChestAnim();
                    opening = true;
                }
            }
            else
            {
                if (distanceX < 0.0f && playerMove->aimAngle <= 0 && playerMove->aimAngle > -180) // chest left
                {
                    playerGunManager = (PlayerGunManager*)other.GetGameObject().GetScript("PlayerGunManager");
                    playerStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");

                    if (playerMove) playerMove->PlayOpenChestAnim();
                    opening = true;
                }
                else if (distanceX >= 0.0f && (playerMove->aimAngle <= -180 || (playerMove->aimAngle < 90 && playerMove->aimAngle >= 0))) // chest right
                {
                    playerGunManager = (PlayerGunManager*)other.GetGameObject().GetScript("PlayerGunManager");
                    playerStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");

                    if (playerMove) playerMove->PlayOpenChestAnim();
                    opening = true;
                }
            }
        }
    }
}

void Chest::OpenChestOnStart()
{
    chestAnimatorPlayer.Play();
    gameObject.SetActive(false);
}