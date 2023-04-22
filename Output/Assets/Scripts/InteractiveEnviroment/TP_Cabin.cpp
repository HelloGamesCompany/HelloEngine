#include "TP_Cabin.h"

HELLO_ENGINE_API_C TP_Cabin* CreateTP_Cabin(ScriptToInspectorInterface* script)
{
	TP_Cabin* classInstance = new TP_Cabin();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

	script->AddDragBoxTransform("Destination: ", &classInstance->destination);
	script->AddDragBoxParticleSystem("Smoke Particle", &classInstance->smoke);

	return classInstance;
}

void TP_Cabin::Start()
{
	destinationSmoke = destination.GetGameObject().AddParticleSystem(smoke);
}
void TP_Cabin::Update()
{
}

void TP_Cabin::OnCollisionEnter(API_RigidBody other)
{

	std::string detectionTag = other.GetGameObject().GetTag();


	if (detectionTag == "Player")
	{

		//INSERT UI

		if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_REPEAT)
		{
			timeHoldButton += Time::GetDeltaTime();
		}
		else
		{
			timeHoldButton -= Time::GetDeltaTime();
			smoke.StopEmitting();
			destinationSmoke.StopEmitting();
		}

		if (1.4f > timeHoldButton && timeHoldButton > 1.0f)
		{
			smoke.Play();
			destinationSmoke.Play();
			Audio::Event("teleport_1");
		}
		else if (timeHoldButton > 1.5f)
		{
			other.GetGameObject().GetTransform().SetPosition(destination.GetGlobalPosition());
			timeHoldButton = 0.0f;
			smoke.StopEmitting();
			destinationSmoke.StopEmitting();
			Audio::Event("teleport_2");
			
		}
	}

}
