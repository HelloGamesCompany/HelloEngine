#include "ArmoryUpgratteButtons.h"
HELLO_ENGINE_API_C ArmoryUpgratteButtons* CreateArmoryUpgratteButtons(ScriptToInspectorInterface* script)
{
	ArmoryUpgratteButtons* classInstance = new ArmoryUpgratteButtons();

	script->AddDragBoxUIButton("Continue", &classInstance->Unlock);
	script->AddDragBoxUIButton("NewGame", &classInstance->Upgrate1);
	script->AddDragBoxUIButton("Exit", &classInstance->Upgrate2);
	script->AddDragBoxUIButton("Settings", &classInstance->Upgrate3);

	return classInstance;
}

void ArmoryUpgratteButtons::Start()
{

}
void ArmoryUpgratteButtons::Update()
{
	if (Unlock.OnPress())
	{

	}

	if (Upgrate1.OnPress())
	{

	}

	if (Upgrate2.OnPress())
	{

	}

	if (Upgrate3.OnPress())
	{

	}
}