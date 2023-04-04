#include "BossHP_HUD.h"
HELLO_ENGINE_API_C BossHP_HUD* CreateBossHP_HUD(ScriptToInspectorInterface* script)
{
	BossHP_HUD* classInstance = new BossHP_HUD();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void BossHP_HUD::Start()
{

}
void BossHP_HUD::Update()
{

}