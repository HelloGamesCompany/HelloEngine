#include "Headers.h"
#include "API_RigidBody.h"
#include "ModuleLayers.h"
#include "PhysicsComponent.h"
#include "API_GameObject.h"

API::API_RigidBody::API_RigidBody()
{
}

API::API_RigidBody::~API_RigidBody()
{
}

API::API_GameObject API::API_RigidBody::GetGameObject()
{
	if (!_rigidBody)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR Rigidbody");
		return *ModuleLayers::emptyAPIGameObject;
	}
	API_GameObject returnGO;
	returnGO.SetGameObject(_rigidBody->GetGameObject());
	return returnGO;
}

PhysicsComponent* API::API_RigidBody::GetComponent()
{
	if (!_rigidBody) 
		Engine::Console::S_Log("Trying to get a NULLPTR Rigidbody");
	
    return _rigidBody;
}

void API::API_RigidBody::SetComponent(PhysicsComponent* component)
{
	_rigidBody = component;
}
