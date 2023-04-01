#include "SmokeExtractorTrap.h"
#include "../Player/PlayerStats.h"

HELLO_ENGINE_API_C SmokeExtractorTrap* CreateSmokeExtractorTrap(ScriptToInspectorInterface* script)
{
	SmokeExtractorTrap* classInstance = new SmokeExtractorTrap();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

	script->AddDragBoxParticleSystem("Smoke Particle", &classInstance->smoke);
	script->AddDragBoxParticleSystem("Fire Particle", &classInstance->fire);
	//Cuanto tarda a empezar a quitar vida
	script->AddDragFloat("Delay on Hit", &classInstance->delay);
	//Valor mínimo para detectar que el jugador esta lejos para reiniciar el delay
	script->AddDragInt("Min Distance Value", &classInstance->minDistance);

	return classInstance;
}

void SmokeExtractorTrap::Start()
{
	smoke.Play();
	playerRef = Game::FindGameObject("Player");

	currentDelay = delay;
}
void SmokeExtractorTrap::Update()
{
	goPos = gameObject.GetTransform().GetGlobalPosition();

	if (goPos.Distance(playerRef.GetTransform().GetGlobalPosition()) > minDistance)
	{
		currentDelay = delay;
		fire.StopEmitting();
		smoke.Play();
	}
}

void SmokeExtractorTrap::OnCollisionEnter(API_RigidBody other)
{

	std::string detectionName = other.GetGameObject().GetName();

	if (detectionName == "Player")
	{

		currentDelay -= Time::GetDeltaTime();
		
		if (currentDelay <= 0.0f)
		{
			smoke.StopEmitting();
			fire.Play();
			
			PlayerStats* playerStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");

			playerStats->TakeDamage(10.0f);

		}
	}

}
