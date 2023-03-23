#include "Temp_Teleporter.h"
HELLO_ENGINE_API_C Temp_Teleporter* CreateTemp_Teleporter(ScriptToInspectorInterface* script)
{
	Temp_Teleporter* classInstance = new Temp_Teleporter();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxTransform("Point 1: ", &classInstance->point1);
	return classInstance;
}

void Temp_Teleporter::Start()
{

}
void Temp_Teleporter::Update()
{

}

void Temp_Teleporter::OnCollisionEnter(API_RigidBody other)
{
	std::string detectionName = other.GetGameObject().GetName();
	if (detectionName == "Player")
	{
		other.GetGameObject().GetTransform().SetPosition(point1.GetGlobalPosition());
	}
}
