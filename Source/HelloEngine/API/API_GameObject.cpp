#include "Headers.h"
#include "API_GameObject.h"
#include "GameObject.h"
#include "Console.h"
#include "ModuleLayers.h"

API::API_GameObject::API_GameObject()
{
	ModuleLayers::apiGameObjects.push_back(this);
}

API::API_GameObject::~API_GameObject()
{
	for (int i = 0; i < ModuleLayers::apiGameObjects.size(); ++i)
	{
		if (ModuleLayers::apiGameObjects[i] == this)
		{
			ModuleLayers::apiGameObjects.erase(ModuleLayers::apiGameObjects.begin() + i);
			break;
		}
	}
}

std::string API::API_GameObject::GetName()
{
	if (_gameObject == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR GameObject!");
		return "NULL";
	}
	return _gameObject->name;
}

std::string API::API_GameObject::GetTag()
{
	if (_gameObject == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR GameObject!");
		return "NULL";
	}
	return _gameObject->tag;
}

API::API_Transform* API::API_GameObject::GetTransform()
{
	if (_gameObject == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR GameObject's transform! Returning empty trasnform.");
		return ModuleLayers::emptyAPITransform;
	}
	return _transform;
}

void API::API_GameObject::SetGameObject(GameObject* gameObject)
{
	_gameObject = gameObject;
}
