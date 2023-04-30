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

    std::string detectionTag = other.GetGameObject().GetTag();
    //Console::Log("hit");

    if (detectionTag == "Projectile")
    {
        Projectile* projectile = (Projectile*)other.GetGameObject().GetScript("Projectile");
        if (projectile->type == PROJECTILE_TYPE::NONE || projectile->type == PROJECTILE_TYPE::AUTO || projectile->type == PROJECTILE_TYPE::SEMI || projectile->type == PROJECTILE_TYPE::SHOTGUN) {
            
        }
        
    }
}