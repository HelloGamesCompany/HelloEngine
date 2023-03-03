#include "PlayerGamepadMovement.h"
HELLO_ENGINE_API_C PlayerGamepadMovement* CreatePlayerGamepadMovement(ScriptToInspectorInterface* script)
{
	PlayerGamepadMovement* classInstance = new PlayerGamepadMovement();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void PlayerGamepadMovement::Start()
{

}
void PlayerGamepadMovement::Update()
{

}