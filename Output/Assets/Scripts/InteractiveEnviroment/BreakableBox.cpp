#include "BreakableBox.h"
#include "../Shooting/Projectile.h"
#include "../Player/PlayerMove.h"
HELLO_ENGINE_API_C BreakableBox* CreateBreakableBox(ScriptToInspectorInterface* script)
{
	BreakableBox* classInstance = new BreakableBox();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxParticleSystem("Box Destroyed Particle", &classInstance->boxDestroyed);
	script->AddDragInt("MaxHp", &classInstance->maxHp);

	return classInstance;
}

void BreakableBox::Start()
{
	srand(time(NULL));

	currentHp = maxHp;
}
void BreakableBox::Update()
{

}

void BreakableBox::OnCollisionEnter(API_RigidBody other)
{

	std::string detectionName = other.GetGameObject().GetName();

	if (detectionName == "Projectile")
	{
		
		Projectile* projectile = (Projectile*)other.GetGameObject().GetScript("Projectile");

		ShootBox(projectile->damage);
		
	}
	else if (detectionName == "Player")
	{
		PlayerMove* playerMove = (PlayerMove*)other.GetGameObject().GetScript("PlayerMove");

		if (playerMove->isDashing)
		{
			DestroyBox();
		}
	} 
}

void BreakableBox::ShootBox(float projectileDamage)
{

    // Health damage
    currentHp -= projectileDamage;
    if (currentHp <= 0)
    {
        currentHp = 0;
        DestroyBox();

		boxDestroyed.Play();

    }

}

void BreakableBox::DestroyBox()
{

	gameObject.SetActive(false);

}

void BreakableBox::CreatePowerUps()
{

	int reward = rand() % 2;

	switch (reward)
	{
	case 0:
	{
		// FIRST AID KIT
		
	}break;
	case 1:
	{
		// AMMO
		
	}break;
	default:
		break;
	}


}
