#include "ThanosDefense.h"
#include "../Shooting/Projectile.h"
HELLO_ENGINE_API_C ThanosDefense* CreateThanosDefense(ScriptToInspectorInterface* script)
{
	ThanosDefense* classInstance = new ThanosDefense();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void ThanosDefense::Start()
{

}
void ThanosDefense::Update()
{

}

void ThanosDefense::OnCollisionEnter(API::API_RigidBody other)
{
    //std::string detectionName = other.GetGameObject().GetName();
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Projectile")
    {
        Console::Log("Tupapasito29");
        other.GetGameObject().GetTransform().Rotate(0, 180, 0);
        Projectile* projectile = (Projectile*)other.GetGameObject().GetScript("Projectile");
        projectile->reflected = true;
    }
}