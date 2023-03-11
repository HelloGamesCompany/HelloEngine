#include "RotateContiniously.h"
HELLO_ENGINE_API_C RotateContiniously* CreateRotateContiniously(ScriptToInspectorInterface* script)
{
	RotateContiniously* classInstance = new RotateContiniously();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void RotateContiniously::Start()
{

}
void RotateContiniously::Update()
{
	gameObject.GetTransform().Rotate(0, 1.0, 0.0f);
}