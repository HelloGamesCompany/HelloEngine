#include "Headers.h"
#include "API_GameObject.h"
#include "GameObject.h"
#include "Console.h"
#include "ModuleLayers.h"
#include "ScriptComponent.h"
#include "API_Transform.h"

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

const char* API::API_GameObject::GetName()
{
	if (_gameObject == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR GameObject! GetName()");
		return "NULL";
	}
	return _gameObject->name.c_str();
}

std::string API::API_GameObject::GetTag()
{
	if (_gameObject == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR GameObject! GetTag()");
		return "NULL";
	}
	return _gameObject->tag;
}

void API::API_GameObject::AddScript(const char* className)
{
	if (_gameObject == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR GameObject! AddScript()");
		return;
	}
	ScriptComponent* scriptComponent = _gameObject->AddComponent<ScriptComponent>();
	scriptComponent->AddScript(className);
}

void API::API_GameObject::Destroy()
{
	if (_gameObject == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR GameObject. Destroy()");
		return;
	}
	_gameObject->Destroy();
}

API::API_Transform API::API_GameObject::GetTransform()
{
	if (_gameObject == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR GameObject. GetTransform()");
		return *ModuleLayers::emptyAPITransform;
	}
	API_Transform returnTransform;
	returnTransform.SetComponent(_gameObject->transform);
	return returnTransform;
}

void API::API_GameObject::SetGameObject(GameObject* gameObject)
{
	_gameObject = gameObject;
}
