#include "Boss_Bar_HUD.h"
HELLO_ENGINE_API_C Boss_Bar_HUD* CreateBoss_Bar_HUD(ScriptToInspectorInterface* script)
{
	Boss_Bar_HUD* classInstance = new Boss_Bar_HUD();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxGameObject("Boss Stats GO", &classInstance->bossStatsGO);

	script->AddDragBoxGameObject("All GO HUD BOSS", &classInstance->boss_HUD);
	script->AddDragBoxUIImage("HP BOSS UI IMAGE", &classInstance->boss_bar);

	script->AddDragBoxTextureResource("HP BOSS 1 TEXTURE", &classInstance->hp_boss_texture);
	script->AddDragBoxTextureResource("SHIELD BOSS 1 TEXTURE", &classInstance->shield_boss_texture[0]);
	script->AddDragBoxTextureResource("SHIELD BOSS 2 TEXTURE", &classInstance->shield_boss_texture[1]);
	script->AddDragBoxTextureResource("SHIELD BOSS 3 TEXTURE", &classInstance->shield_boss_texture[2]);

	
	return classInstance;
}

void Boss_Bar_HUD::Start()
{
	bossStats = (BossLoop*)bossStatsGO.GetScript("BossLoop");
	if (bossStats == nullptr) Console::Log("Missing BossStats on Boss_Bar_HUD Script.");

	boss_bar.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(shield_boss_texture[0]);

	boss_HUD.SetActive(true);
	
}
void Boss_Bar_HUD::Update()
{
	float shields[3] = {0,0,0};

	shields[0] = bossStats->shield[0];
	shields[1] = bossStats->shield[1];
	shields[2] = bossStats->shield[2];

	if (bossStats != nullptr)
	{
		Console::Log("hola");
		Boss_Bar(bossStats->hp / 1500, shields, bossStats->phase, bossStats->canTakeDamage);
	}
	else
	{
		Console::Log("polla");
	}
}

void Boss_Bar_HUD::Boss_Bar(float HP_Boss_Value, float Shield_Boss_Value[3], int phase, bool Is_Active)
{
	Shield_Boss_Value[phase] = Shield_Boss_Value[phase] / bossStats->maxShield[phase];

	if (Is_Active == true) {
		boss_bar.FillImage(HP_Boss_Value);
		boss_bar.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(hp_boss_texture);
	}
	else
	{
		boss_bar.FillImage(Shield_Boss_Value[phase]);
		boss_bar.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(shield_boss_texture[phase]);
	}
}
