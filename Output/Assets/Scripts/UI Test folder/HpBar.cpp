#include "HpBar.h"
HELLO_ENGINE_API_C HpBar* CreateHpBar(ScriptToInspectorInterface* script)
{
    HpBar* classInstance = new HpBar();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Player Stats GO", &classInstance->playerStatsGO);
    return classInstance;
}

void HpBar::Start()
{
    playerStats = (PlayerStats*)playerStatsGO.GetScript("PlayerStats");
    if (playerStats == nullptr) Console::Log("Missing PlayerStats on HpBar Script.");

    FirstPosX = gameObject.GetTransform().GetLocalPosition().x;
    FirstPosY = gameObject.GetTransform().GetLocalPosition().y;
    FirstPosZ = gameObject.GetTransform().GetLocalPosition().z;
}
void HpBar::Update()
{
    float finalScale;
    if (playerStats) finalScale = playerStats->currentHp * 0.4f / playerStats->currentMaxHp;
    else finalScale = 100.0f * 0.4f / 100.0f;

    if (finalScale != gameObject.GetTransform().GetLocalScale().x) {

        float differenceHp = playerStats->currentMaxHp - playerStats->currentHp;

        float offsetX = differenceHp * 0.0036f;
        //float offsetY = differenceHp * 0.0014f;

        //gameObject.GetTransform().SetPosition(FirstPosX - offsetX, FirstPosY - offsetY, gameObject.GetTransform().GetLocalPosition().z);
        gameObject.GetTransform().SetPosition(FirstPosX - offsetX, FirstPosY, gameObject.GetTransform().GetLocalPosition().z);
    }

    gameObject.GetTransform().SetScale(finalScale, 0.12f, 0.5f);
}