#include "Rotation.h"
HELLO_ENGINE_API_C Rotation* CreateRotation(ScriptToInspectorInterface* script)
{
	Rotation* classInstance = new Rotation();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragFloat("Velocity", &classInstance->vel);
	return classInstance;
}

void Rotation::Start()
{

}
void Rotation::Update()
{
	gameObject.GetTransform().Rotate(API_Vector3(0,0,vel));
}