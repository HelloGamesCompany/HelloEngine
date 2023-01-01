#pragma once
#include "Globals.h"
#include "API_GameObject.h"

class TransformComponent;

namespace API
{
	class TO_API API_Transform
	{
	public:
		API_Transform();
		~API_Transform();

		void Translate(float x, float y, float z);
		/// WARNING: Do not destroy the API_Transform after using this. The API_GameObject returned is inside this class.
		API_GameObject& GetGameObject();

	private:
		void SetComponent(TransformComponent* transformComponent);
	private:
		API_GameObject _APIGameObject;
		TransformComponent* _transform = nullptr;

		friend class DragBoxTransform;
	};
}
