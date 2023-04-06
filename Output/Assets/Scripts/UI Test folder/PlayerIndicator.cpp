#include "PlayerIndicator.h"
HELLO_ENGINE_API_C PlayerIndicator* CreatePlayerIndicator(ScriptToInspectorInterface* script)
{
    PlayerIndicator* classInstance = new PlayerIndicator();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Player", &classInstance->player);
    script->AddDragFloat("Movemnet Reduction X", &classInstance->movementReductionX);
    script->AddDragFloat("Movemnet Reduction Y", &classInstance->movementReductionY);
    return classInstance;
}

void PlayerIndicator::Start()
{
    originIndicatorPos = API_Vector2(gameObject.GetTransform().GetGlobalPosition().x, gameObject.GetTransform().GetGlobalPosition().y);
    originPlayerPos = API_Vector2(player.GetTransform().GetGlobalPosition().x, player.GetTransform().GetGlobalPosition().z);
}

void PlayerIndicator::Update()
{
    API_Vector2 variation = originPlayerPos - API_Vector2(player.GetTransform().GetGlobalPosition().x, player.GetTransform().GetGlobalPosition().z);
    float newPosX = originIndicatorPos.x + (variation.x / movementReductionX);
    float newPosY = originIndicatorPos.y - (variation.y / movementReductionY);
    gameObject.GetTransform().SetPosition(newPosX, newPosY, 0.0f);
}