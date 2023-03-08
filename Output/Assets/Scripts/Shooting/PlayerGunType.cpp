#include "PlayerGunType.h"
HELLO_ENGINE_API_C PlayerGunType* CreatePlayerGunType(ScriptToInspectorInterface* script)
{
    PlayerGunType* classInstance = new PlayerGunType();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragInt("Gun Type", &classInstance->gunType);
    return classInstance;
}

void PlayerGunType::Start()
{

}
void PlayerGunType::Update()
{

}