#include "Headers.h"
#include "API_Transform.h"
#include "TransformComponent.h"
#include "ModuleLayers.h"

API::API_Transform::API_Transform()
{
}

API::API_Transform::~API_Transform()
{
}

void API::API_Transform::Translate(float x, float y, float z)
{
	if (_transform == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR Transform!");
		return;
	}
	_transform->Translate(float3(x, y, z));
}

void API::API_Transform::Rotate(float x, float y, float z)
{
	if (_transform == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR Transform!");
		return;
	}
	_transform->Rotate(float3(x, y, z));
}

void API::API_Transform::Scale(float x, float y, float z)
{
	if (_transform == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR Transform!");
		return;
	}
	_transform->Scale(float3(x, y, z));
}

void API::API_Transform::SetPosition(float x, float y, float z)
{
	if (_transform == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR Transform!");
		return;
	}
	_transform->SetPosition(float3(x, y, z));
}

void API::API_Transform::SetRotation(float x, float y, float z)
{
	if (_transform == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR Transform!");
		return;
	}
	_transform->SetRotation(float3(x, y, z));
}

void API::API_Transform::SetScale(float x, float y, float z)
{
	if (_transform == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR Transform!");
		return;
	}
	_transform->SetScale(float3(x, y, z));
}

API::API_GameObject API::API_Transform::GetGameObject()
{
	if (_transform == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR GameObject. GetTransform()");
		return *ModuleLayers::emptyAPIGameObject;
	}
	API_GameObject returnGO;
	returnGO.SetGameObject(_transform->GetGameObject());
	return returnGO;
}

void API::API_Transform::SetComponent(TransformComponent* transformComponent)
{
	_transform = transformComponent;
}
