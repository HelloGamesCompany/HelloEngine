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

void API::API_RigidBody::SetVelocity(API_Vector3 vec)
{
	if (!_rigidBody)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR Rigidbody");
		return;
	}	

	_rigidBody->SetVelocity(vec);
}

API::API_Vector3 API::API_RigidBody::GetVelocity()
{
	if (!_rigidBody)
	{
		Engine::Console::S_Log("Trying to get a NULLPTR Rigidbody");
		return API_Vector3::S_Zero();
	}
	return _rigidBody->GetVelocity();
}

void API::API_RigidBody::SetGravity(API_Vector3 gravity)
{
	if (!_rigidBody)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR Rigidbody");
		return;
	}

	_rigidBody->SetGravity(gravity);
}

API::API_Vector3 API::API_RigidBody::GetGravity()
{
	if (!_rigidBody)
	{
		Engine::Console::S_Log("Trying to get a NULLPTR Rigidbody");
		return API_Vector3::S_Zero();
	}
	return _rigidBody->GetGravity();
}

float API::API_RigidBody::GetRadius()
{
	if (!_rigidBody)
	{
		Engine::Console::S_Log("Trying to get a NULLPTR Rigidbody");
		return 0;
	}

	float ret = _rigidBody->GetRadius();

	if (ret <= 0) 
	{
		Engine::Console::S_Log("Trying to get a radius a non sphere rigidbody!!!");
	}

	return ret;
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
