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

}

API::API_GameObject::~API_GameObject()
{
	if (_gameObject != nullptr)
	{
		ModuleLayers::apiGameObjects.erase(_gameObject->GetID());
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

const char* API::API_GameObject::GetTag()
{
	if (_gameObject == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR GameObject! GetTag()");
		return "NULL";
	}
	return _gameObject->tag.c_str();
}

HelloBehavior* API::API_GameObject::AddScript(const char* className)
{
	if (_gameObject == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR GameObject! AddScript()");
		return nullptr;
	}
	ScriptComponent* scriptComponent = _gameObject->AddComponent<ScriptComponent>();
	scriptComponent->AddScript(className);
	return scriptComponent->GetScript();
}

HelloBehavior* API::API_GameObject::GetScript(const char* className)
{
	if (_gameObject == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR GameObject! GetScript()" + std::string(className));
		return nullptr;
	}
	std::vector<ScriptComponent*> scripts;
	std::vector<Component*> components = _gameObject->GetComponents();
	std::string name(className); // Needs a string to compare to another string.

	for (int i = 0; i < components.size(); ++i)
	{
		if (components[i]->GetType() == Component::Type::SCRIPT)
			scripts.push_back((ScriptComponent*)components[i]);
	}

	for (int i = 0; i < scripts.size(); ++i)
	{
		if (scripts[i]->GetScriptName() == className)
		{
			return scripts[i]->GetScript();
		}
	}

	return nullptr;
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

void API::API_GameObject::SetActive(bool active)
{
	if (_gameObject == nullptr)
	{
		Console::S_Log("Trying to acces a NULLPTR GameObject. SetActive()");
		return;
	}
	_gameObject->SetActive(active);
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
	if (_gameObject != nullptr)
		ModuleLayers::apiGameObjects[gameObject->GetID()] = this;
}
