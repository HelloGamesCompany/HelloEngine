#include "HUD_Shield_Indicator.h"
HELLO_ENGINE_API_C HUD_Shield_Indicator* CreateHUD_Shield_Indicator(ScriptToInspectorInterface* script)
{
	HUD_Shield_Indicator* classInstance = new HUD_Shield_Indicator();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);



	return classInstance;
}

void HUD_Shield_Indicator::Start()
{

}
void HUD_Shield_Indicator::Update()
{


}


