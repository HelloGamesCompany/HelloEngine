#pragma once
#include "Globals.h"
#include <string>

class GameObject;
class DragBoxGameObject;
class LayerGame;

namespace API
{
	class API_Transform;
	class TO_API API_GameObject
	{
	public:
		API_GameObject();
		~API_GameObject();

		// Any necessary methods from the GameObject class, but with a nullptr check before calling.

		const char* GetName();
		std::string GetTag();

		void AddScript(const char* className);

		// Destroys this GameObject instance from the scene. You can still use the API_GameObject class, but it wont make any effect.
		void Destroy();

		bool IsAlive() { return _gameObject != nullptr; }

		/// Call this function to get an API_Transform pointer. WARNING: You should use this 
		API::API_Transform GetTransform();

	private:
		GameObject* GetGameObject() { return _gameObject; };
		void SetGameObject(GameObject* gameObject);
	private:
		GameObject* _gameObject = nullptr;
		friend class ModuleLayers;
		friend class DragBoxGameObject;
		friend class API_Transform;
		friend class Game;
		friend class LayerGame;
	};
}