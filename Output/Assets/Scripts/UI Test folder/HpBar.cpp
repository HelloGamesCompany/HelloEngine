#include "HpBar.h"
HELLO_ENGINE_API_C HpBar* CreateHpBar(ScriptToInspectorInterface* script)
{
    HpBar* classInstance = new HpBar();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Player Stats GO", &classInstance->playerStatsGO);
    script->AddDragBoxUIImage("HP_BAR", &classInstance->hp_Bar);
    script->AddDragBoxUIImage("HP_REGEN_BAR", &classInstance->hp__Regen_Bar);
    script->AddDragBoxUIImage("SHIELD_BAR", &classInstance->shield_Bar);
    return classInstance;
}

void HpBar::Start()
{
    playerStats = (PlayerStats*)playerStatsGO.GetScript("PlayerStats");
    if (playerStats == nullptr) Console::Log("Missing PlayerStats on HpBar Script.");

    hp_Bar.FillImage(0.5);
    hp__Regen_Bar.GetGameObject().SetActive(false);
    shield_Bar.FillImage(0);

}
void HpBar::Update()
{

}

void HpBar::HpPlayerbar(float vida_Player_value, float Regen_Player_value, bool Regen_Bar_Active)
{

    if (Regen_Bar_Active == false)
    {
        hp__Regen_Bar.GetGameObject().SetActive(false);
        hp_Bar.GetGameObject().SetActive(true);
        Regen_Player_value = vida_Player_value;
        hp_Bar.FillImage(vida_Player_value);
    }
    
    if (Regen_Bar_Active == true)
    {
        hp__Regen_Bar.GetGameObject().SetActive(true);
        hp_Bar.GetGameObject().SetActive(false);
        vida_Player_value = Regen_Player_value;
        hp__Regen_Bar.FillImage(Regen_Player_value);
    }
}

void HpBar::ShieldPlayerbar(float Shield_Player_value, bool Shield_Active)
{
    if (Shield_Active == true)
    {
        shield_Bar.FillImage(Shield_Player_value);
    }
    
    if (Shield_Active == false)
    {
        shield_Bar.FillImage(0);
    }
}
