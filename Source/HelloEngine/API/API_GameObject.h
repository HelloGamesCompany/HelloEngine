#pragma once
#include "Globals.h"

class GameObject;
class DragBoxGameObject;

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

		/// Call this function to get an API_Transform pointer. WARNING: You should use this 
		API::API_Transform* GetTransform();

	private:
		void SetGameObject(GameObject* gameObject);
	private:
		GameObject* _gameObject = nullptr;
		API_Transform* _transform = nullptr;
		friend class ModuleLayers;
		friend class DragBoxGameObject;
		friend class API_Transform;
	};
}