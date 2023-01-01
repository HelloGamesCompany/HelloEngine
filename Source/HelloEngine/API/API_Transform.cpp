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
	if (_APIGameObject._gameObject == nullptr || _transform == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR Transform!");
		return;
	}
	_transform->Translate(float3(x, y, z));
}

API::API_GameObject& API::API_Transform::GetGameObject()
{
	if (_APIGameObject._gameObject == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR GameObject! (API_Transform::GetGameObject())");
		return *ModuleLayers::emptyAPIGameObject;
	}
	return _APIGameObject;
}

void API::API_Transform::SetComponent(TransformComponent* transformComponent)
{
	_transform = transformComponent;
	_APIGameObject.SetGameObject(transformComponent->GetGameObject());
}
