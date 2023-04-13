#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "API/API_UIImage.h"
#include "../EbonyMaw/BossLoop.h"

#include "API/API.h"

class Boss_Bar_HUD : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void Boss_Bar(float HP_Boss_Value, float Shield_Boss_Value[3], int phase, bool Is_Active);

	API_GameObject bossStatsGO;
	BossLoop* bossStats;

	uint shield_boss_texture[3];
	uint hp_boss_texture;

	API_GameObject boss_HUD;
	API_UIImage boss_hp_bar;
	API_UIImage boss_shield_bar;
};

