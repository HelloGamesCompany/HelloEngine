#include "HouseBehavior.h"
HELLO_ENGINE_API_C HouseBehavior* CreateHouseBehavior(ScriptToInspectorInterface* script)
{
	HouseBehavior* classInstance = new HouseBehavior();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void HouseBehavior::Start()
{

}
void HouseBehavior::Update()
{
	this->gameObject.GetTransform().Translate(0, 0.01f, 0);
}