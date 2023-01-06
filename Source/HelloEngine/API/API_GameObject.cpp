#include "Headers.h"
#include "API_GameObject.h"
#include "GameObject.h"
#include "Console.h"
#include "ModuleLayers.h"
#include "ScriptComponent.h"
#include "API_Transform.h"
#include "API_MeshRenderer.h"

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

API::API_MeshRenderer API::API_GameObject::AddMeshRenderer()
{
	if (_gameObject == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR GameObject! AddMeshRenderer()");
		return API_MeshRenderer();
	}
	MeshRenderComponent* component = (MeshRenderComponent*)_gameObject->AddComponentOfType(Component::Type::MESH_RENDERER);
	if (component == nullptr)
	{
		Console::S_Log("Trying to add a second MeshRender to a GameObject. Only one MeshRender for game object is allowed. AddMeshRenderer()");
		return API_MeshRenderer();
	}
	API_MeshRenderer ret;
	ret.SetComponent(component);
	return ret;
}

API::API_MeshRenderer API::API_GameObject::AddMeshRenderer(API_MeshRenderer& copy)
{
	if (_gameObject == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR GameObject! AddMeshRenderer()");
		return API_MeshRenderer();
	}
	if (copy._meshRenderer == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR MeshRenderComponent! AddMeshRenderer()");
		return API_MeshRenderer();
	}
	MeshRenderComponent* component = (MeshRenderComponent*)_gameObject->AddComponentOfType(Component::Type::MESH_RENDERER, *(Component*)copy._meshRenderer);
	
	if (component == nullptr)
	{
		Console::S_Log("Trying to add a second MeshRender to a GameObject. Only one MeshRender for game object is allowed. AddMeshRenderer()");
		return API_MeshRenderer();
	}
	
	API_MeshRenderer ret;
	ret.SetComponent(component);
	return ret;
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
