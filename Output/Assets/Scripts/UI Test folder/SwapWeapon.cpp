#include "SwapWeapon.h"
HELLO_ENGINE_API_C SwapWeapon* CreateSwapWeapon(ScriptToInspectorInterface* script)
{
	SwapWeapon* classInstance = new SwapWeapon();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void SwapWeapon::Start()
{

}
void SwapWeapon::Update()
{

}