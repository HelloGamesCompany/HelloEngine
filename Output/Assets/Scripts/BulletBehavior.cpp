#include "BulletBehavior.h"

HELLO_ENGINE_API_C BulletBehavior* CreateBulletBehavior(ScriptToInspectorInterface* script)
{
	BulletBehavior* classInstance = new BulletBehavior();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragFloat("Lifetime", &classInstance->lifeTime);
	return classInstance;
}

void BulletBehavior::Start()
{

}
void BulletBehavior::Update()
{
	lifeTime -= Time::GetDeltaTime();

	if (lifeTime <= 0)
	{
		this->gameObject.Destroy();
	}

	this->gameObject.GetTransform().Translate(direction);
}