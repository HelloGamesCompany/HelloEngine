#pragma once
#include "Component.h"
#include "TransformComponent.h"

class GameObject
{
public:
	GameObject(GameObject* parent, std::string name = "Default", std::string tag = "Default");
	GameObject(GameObject* parent, std::string& name, std::string& tag);
	~GameObject();

	//TODO: Could do with an array of functions that creates a specific component
	template <class T>
	T* AddComponent()
	{
		T* newComponent = new T(this);
		_components.push_back(newComponent);

		return newComponent;
	}

	template<class T>
	T* GetComponent(uint index = 0)
	{
		uint counter = 0;
		for (const auto& component : _components)
		{
			if (typeid(*component) != typeid(T)) continue;
	
			if (counter == index) return (T*)component;
			else counter++;
		}
		return nullptr;
	}

	std::vector<Component*> GetComponents() { return _components;}

	GameObject* GetParent() { return _parent; }

	bool AddChild(GameObject* child);

	bool SetParent(GameObject* parent);

	void SetActive(bool active);

	bool IsActive() { return _isActive; }

	bool IsStatic() { return _isStatic; }

	std::vector<GameObject*>* GetChildren() { return &_children; }

	void OnEditor();

	template <class T>
	bool HasComponent()
	{
		for (const auto& component : _components)
		{
			if (typeid(*component) != typeid(T)) continue;

			return true;
		}
		return false;
	}

public:
	std::string name = "";
	std::string tag = "";
	TransformComponent* transform = nullptr;

private:
	void RemoveChild(GameObject* child);

private:
	std::vector<Component*> _components;
	std::vector<GameObject*> _children;

	GameObject* _parent = nullptr;

	bool _isActive = true;
	bool _isDestroyed = false;
	bool _isStatic = false;

	uint _ID = 0; // ID =  0 is an invalid ID. First ID is 1.

	// On Editor variables
	std::string comboValues[2] = {"Mesh Renderer", "Material"};

	friend class TransformComponent;
	friend class ImWindowHierarchy;
};