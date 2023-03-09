#pragma once
#include "Globals.h"
#include <string>

class GameObject;
class DragBoxGameObject;
class LayerGame;
class HelloBehavior;

namespace API
{
	class API_Transform;
	class API_MeshRenderer;
	class API_Camera;
	class API_RigidBody;
	class API_AnimationPlayer;
	class TO_API API_GameObject
	{
	public:
		API_GameObject();
		~API_GameObject();

		// Any necessary methods from the GameObject class, but with a nullptr check before calling.

		const char* GetName();
		const char* GetTag();

		HelloBehavior* AddScript(const char* className);
		HelloBehavior* GetScript(const char* className);

		API_MeshRenderer AddMeshRenderer();
		API_MeshRenderer AddMeshRenderer(API_MeshRenderer& copy);

		void SetActive(bool active);

		// Destroys this GameObject instance from the scene. You can still use the API_GameObject class, but it wont make any effect.
		void Destroy();

		bool IsAlive() { return _gameObject != nullptr; }

		API::API_Transform GetTransform();

	private:
		GameObject* GetGameObject() { return _gameObject; };
		void SetGameObject(GameObject* gameObject);
	private:
		GameObject* _gameObject = nullptr;
		friend class ModuleLayers;
		friend class DragBoxGameObject;
		friend class API_Transform;
		friend class API_MeshRenderer;
		friend class API_RigidBody;
		friend class API_Camera;
		friend class API_AnimationPlayer;
		friend class Game;
		friend class LayerGame;
	};
}