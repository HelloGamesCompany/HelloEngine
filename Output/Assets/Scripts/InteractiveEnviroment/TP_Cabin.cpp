#include "TP_Cabin.h"

HELLO_ENGINE_API_C TP_Cabin* CreateTP_Cabin(ScriptToInspectorInterface* script)
{
	TP_Cabin* classInstance = new TP_Cabin();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

	script->AddDragBoxTransform("Point 1: ", &classInstance->destination);
	script->AddDragBoxParticleSystem("Smoke Particle", &classInstance->smoke);

	return classInstance;
}

void TP_Cabin::Start()
{
	
}
void TP_Cabin::Update()
{
}

void TP_Cabin::OnCollisionEnter(API_RigidBody other)
{

	std::string detectionName = other.GetGameObject().GetName();


	if (detectionName == "Player")
	{
		if (Input::GetKey(KeyCode::KEY_1) == KeyState::KEY_REPEAT)
		{
			timeHoldButton += Time::GetDeltaTime();
		}
		else
		{
			timeHoldButton -= Time::GetDeltaTime();
		}

		if (1.9f > timeHoldButton && timeHoldButton > 1.4f)
		{ 
			smoke.Play();
		}
		else if (timeHoldButton > 2.0f)
		{
			other.GetGameObject().GetTransform().SetPosition(destination.GetGlobalPosition());
			timeHoldButton = 0.0f;
			smoke.StopEmitting();
		}
	} 
	
}
