#pragma once

class GameObject;

class Component
{
public:
	enum class Type
	{
		NONE,
		TRANSFORM,
		MESH_RENDERER,
		MAX
	};

	Component(GameObject* gameObject);
	virtual ~Component() { std::cout << "Destroy Component" << std::endl; }

	GameObject* GetGameObject() { return _gameObject; }
	Type GetType() { return _type; }
	bool IsEnabled(){ return _isEnabled; }

private:
	bool _isEnabled = true;
	GameObject* _gameObject = nullptr;
	Type _type = Type::NONE;

};

