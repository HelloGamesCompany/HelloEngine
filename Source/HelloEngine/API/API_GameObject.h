#pragma once
#include "Globals.h"

class GameObject;

namespace API
{
	class TO_API API_GameObject
	{
	public:
		API_GameObject();
		~API_GameObject();

		// Any necessary methods from the GameObject class, but with a nullptr check before calling.

		std::string GetName();
		std::string GetTag();

	private:
		void SetGameObject(GameObject* gameObject);
	private:
		GameObject* _gameObject = nullptr;
		
		friend class ModuleLayers;
	};
}