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
		MATERIAL,
		MAX
	};

	Component(GameObject* gameObject);
	virtual ~Component() { std::cout << "Destroy Component" << std::endl; }

	GameObject* GetGameObject() { return _gameObject; }
	Type GetType() { return _type; }
	bool IsEnabled(){ return _isEnabled; }

	virtual void Enable(bool enabled, bool fromGo = true) { _isEnabled = enabled; }

	virtual void OnPositionUpdate(float3 pos) {}
	virtual void OnScaleUpdate(float3 scale) {}
	virtual void OnRotationUpdate(float3 rotation) {}
	virtual void OnTransformUpdate(float3 pos, float3 scale, float3 rotation) {}

	virtual void OnEditor() {};

protected:
	bool _isEnabled = true;
	GameObject* _gameObject = nullptr;
	Type _type = Type::NONE;

};