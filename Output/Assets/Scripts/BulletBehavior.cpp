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
	API_Vector3 vector = { 1,2,3 };

	Console::Log("X: " + std::to_string(vector.x) + "Y: " + std::to_string(vector.y) + "Z: " + std::to_string(vector.z));
}
void BulletBehavior::Update()
{

}

void BulletBehavior::OnCollisionEnter(API_RigidBody other)
{
	Console::Log("Collision");
}
