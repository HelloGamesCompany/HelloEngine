#include "Headers.h"
#include "API_Transform.h"
#include "TransformComponent.h"

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

void API::API_Transform::SetComponent(TransformComponent* transformComponent)
{
	_transform = transformComponent;
}
