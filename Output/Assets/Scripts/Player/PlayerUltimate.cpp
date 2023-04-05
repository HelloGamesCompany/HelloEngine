#include "PlayerUltimate.h"
HELLO_ENGINE_API_C PlayerUltimate* CreatePlayerUltimate(ScriptToInspectorInterface* script)
{
    PlayerUltimate* classInstance = new PlayerUltimate();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Player Stats GO", &classInstance->playerStatsGO);
    return classInstance;
}

void PlayerUltimate::Start()
{
    playerStats = (PlayerStats*)playerStatsGO.GetScript("PlayerStats");
    triggerUlt = false;
}

void PlayerUltimate::Update()
{
    if (playerStats->specialTreeLvl < 3) return;

    if (triggerUlt) triggerUlt = false;

    if (Input::GetKey(KeyCode::KEY_F) == KeyState::KEY_DOWN)
    {
        UseUltimate();
    }

    if (Input::GetGamePadButton(GamePadButton::BUTTON_LEFT_STICK) == KeyState::KEY_DOWN)
    {
        if (bufferRJ > 0)
        {
            UseUltimate();
            bufferRJ = 0.0f;
        }
        else bufferLJ = 0.1f;
    }
    if (Input::GetGamePadButton(GamePadButton::BUTTON_RIGHT_STICK) == KeyState::KEY_DOWN)
    {
        if (bufferLJ > 0.0f)
        {
            UseUltimate();
            bufferLJ = 0.0f;
        }
        else bufferRJ = 0.1f;
    }
    if (bufferLJ > 0.0f)
    {
        bufferLJ -= Time::GetRealTimeDeltaTime();
        if (bufferLJ <= 0.0f)
        {
            bufferLJ = 0.0f;
        }
    }
    if (bufferRJ > 0.0f)
    {
        bufferRJ -= Time::GetRealTimeDeltaTime();
        if (bufferRJ <= 0)
        {
            bufferRJ = 0.0f;
        }
    }
}

void PlayerUltimate::OnCollisionEnter(API::API_RigidBody other)
{
    if (triggerUlt) return;
    std::string detectionName = other.GetGameObject().GetName();

    if (detectionName == "Enemy")
    {
        // freeze enemy
    }
}

void PlayerUltimate::UseUltimate()
{
    triggerUlt = true;
}