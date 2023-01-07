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
		Console::S_Log("Trying to acces a NULLPTR Transform! Translate()");
		return;
	}
	_transform->Translate(float3(x, y, z));
}

void API::API_Transform::Rotate(float x, float y, float z)
{
	if (_transform == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR Transform! Rotate()");
		return;
	}
	_transform->Rotate(float3(x, y, z));
}

void API::API_Transform::Scale(float x, float y, float z)
{
	if (_transform == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR Transform! Scale()");
		return;
	}
	_transform->Scale(float3(x, y, z));
}

void API::API_Transform::SetPosition(float x, float y, float z)
{
	if (_transform == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR Transform! SetPosition()");
		return;
	}
	_transform->SetPosition(float3(x, y, z));
}

void API::API_Transform::SetRotation(float x, float y, float z)
{
	if (_transform == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR Transform! SetRotation()");
		return;
	}
	_transform->SetRotation(float3(x, y, z));
}

void API::API_Transform::SetScale(float x, float y, float z)
{
	if (_transform == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR Transform! SetScale()");
		return;
	}
	_transform->SetScale(float3(x, y, z));
}

void API::API_Transform::Translate(API_Vector3 translation)
{
	if (_transform == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR Transform! Translate()");
		return;
	}
	_transform->Translate(translation);
}

void API::API_Transform::Rotate(API_Vector3 rotation)
{
	if (_transform == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR Transform! Rotate()");
		return;
	}
	_transform->Rotate(rotation);
}

void API::API_Transform::Scale(API_Vector3 scalation)
{
	if (_transform == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR Transform! Scale()");
		return;
	}
	_transform->Scale(scalation);
}

void API::API_Transform::SetPosition(API_Vector3 position)
{
	if (_transform == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR Transform! SetPosition()");
		return;
	}
	_transform->SetPosition(position);
}

void API::API_Transform::SetRotation(API_Vector3 rotation)
{
	if (_transform == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR Transform! SetRotation()");
		return;
	}
	_transform->SetRotation(rotation);
}

void API::API_Transform::SetScale(API_Vector3 scale)
{
	if (_transform == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR Transform! SetScale()");
		return;
	}
	_transform->SetScale(scale);
}

API::API_Vector3 API::API_Transform::GetPosition()
{
	if (_transform == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR Transform! GetPosition().");
		return API::API_Vector3();
	}
	return _transform->GetPosition();
}

API::API_Vector3 API::API_Transform::GetRotation()
{
	if (_transform == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR Transform! GetRotation().");
		return API::API_Vector3();
	}
	return _transform->GetRotation();
}

API::API_Vector3 API::API_Transform::GetScale()
{
	if (_transform == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR Transform! GetScale().");
		return API::API_Vector3();
	}
	return _transform->GetScale();
}

API::API_Vector3 API::API_Transform::GetForward()
{
	if (_transform == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR Transform! GetForward().");
		return API::API_Vector3();
	}
	return _transform->GetForward();
}

API::API_Vector3 API::API_Transform::GetRight()
{
	if (_transform == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR Transform! GetRight().");
		return API::API_Vector3();
	}
	return _transform->GetRight();
}

API::API_Vector3 API::API_Transform::GetUp()
{
	if (_transform == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR Transform! GetUp().");
		return API::API_Vector3();
	}
	return _transform->GetUp();
}

API::API_Vector3 API::API_Transform::GetBackward()
{
	if (_transform == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR Transform! GetBackward().");
		return API::API_Vector3();
	}
	return -_transform->GetForward();
}

API::API_Vector3 API::API_Transform::GetLeft()
{
	if (_transform == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR Transform! GetLeft().");
		return API::API_Vector3();
	}
	return -_transform->GetRight();
}

API::API_Vector3 API::API_Transform::GetDown()
{
	if (_transform == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR Transform! GetDown().");
		return API::API_Vector3();
	}
	return -_transform->GetUp();
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
