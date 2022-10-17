#include "Headers.h"
#include "GameObject.h"
#include "ModuleLayers.h"
#include "TransformComponent.h"

GameObject::GameObject(GameObject* parent, std::string name, std::string tag) : name(name), tag(tag)
{
	_ID = Application::Instance()->layers->AddGameObject(this);
	transform = AddComponent<TransformComponent>();
	if (parent != nullptr) parent->AddChild(this);

}

GameObject::GameObject(GameObject* parent, std::string& name, std::string& tag) : name(name), tag(tag)
{
	_ID = Application::Instance()->layers->AddGameObject(this);
	transform = AddComponent<TransformComponent>();
	if (parent != nullptr) parent->AddChild(this);
}

GameObject::~GameObject()
{
	for (int i = 0; i < _components.size(); i++)
	{
		RELEASE(_components[i]);
	}
	_components.clear();

	for (int i = 0; i < _children.size(); i++)
	{
		RELEASE(_children[i]);
	}
	_children.clear();
}

bool GameObject::AddChild(GameObject* child)
{
	if (!child) return false;
	if (child->_parent == this) return false;

	GameObject* p = _parent;

	while (p)
	{
		if (p == child) return false;

		p = p->_parent;
	}

	_children.push_back(child);

	if (child->_parent) child->_parent->RemoveChild(child);

	child->_parent = this;

	child->transform->parentGlobalTransform = transform->GetGlobalTransform(); // Set the child parentGlobalTransform to this trasnform's global transfrom.

	return true;
}

bool GameObject::SetParent(GameObject* parent)
{
	return parent->AddChild(this);
}

void GameObject::RemoveChild(GameObject* child)
{
	if (!child) return;

	for (int i = 0; i < _children.size(); ++i)
	{
		if (_children[i] == child) _children.erase(_children.begin() + i);
	}
	child->_parent = nullptr;
}
