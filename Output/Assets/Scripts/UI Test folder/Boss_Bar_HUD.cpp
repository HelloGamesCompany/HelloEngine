#include "Boss_Bar_HUD.h"
HELLO_ENGINE_API_C Boss_Bar_HUD* CreateBoss_Bar_HUD(ScriptToInspectorInterface* script)
{
	Boss_Bar_HUD* classInstance = new Boss_Bar_HUD();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxGameObject("All GO HUD BOSS", &classInstance->boss_HUD);
	script->AddDragBoxUIImage("HP BOSS", &classInstance->boss_bar);
	return classInstance;
}

void Boss_Bar_HUD::Start()
{
	boss_HUD.SetActive(false);
}
void Boss_Bar_HUD::Update()
{

}

void Boss_Bar_HUD::Boss_Bar(float HP_Boss_Value, bool Is_Active)
{
	if (Is_Active == true) {
		boss_HUD.SetActive(true);
	}
	else if(Is_Active == false)
	{
		boss_HUD.SetActive(false);
	}

	boss_bar.FillImage(HP_Boss_Value);
}
