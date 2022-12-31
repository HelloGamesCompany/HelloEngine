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

	private:
		void SetComponent(TransformComponent* transformComponent);
	private:
		API_GameObject* _APIGameObject = nullptr;
		TransformComponent* _transform = nullptr;
	};
}
