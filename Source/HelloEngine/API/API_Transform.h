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
		void Rotate(float x, float y, float z);
		void Scale(float x, float y, float z);

		void SetPosition(float x, float y, float z);
		void SetRotation(float x, float y, float z);
		void SetScale(float x, float y, float z);

		//TODO: Create this methods when the Vector3 struct is created.
		//GetPosition, GetRotation, GetScale.



		/// WARNING: Do not destroy the API_Transform after using this. The API_GameObject returned is inside this class.
		API_GameObject GetGameObject();

	private:
		void SetComponent(TransformComponent* transformComponent);
	private:
		TransformComponent* _transform = nullptr;

		friend class DragBoxTransform;
		friend class API_GameObject;
	};
}
