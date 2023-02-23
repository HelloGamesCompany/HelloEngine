#pragma once
#include "Globals.h"
#include "API_GameObject.h"
#include "API_Vector3.h"

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

		void Translate(API_Vector3 translation);
		void Rotate(API_Vector3 rotation);
		void Scale(API_Vector3 scalation);

		void SetPosition(API_Vector3 position);
		void SetRotation(API_Vector3 rotation);
		void SetScale(API_Vector3 scale);

		API_Vector3 GetLocalPosition();
		API_Vector3 GetLocalRotation();
		API_Vector3 GetLocalScale();

		API_Vector3 GetGlobalPosition();
		API_Vector3 GetGlobalRotation();
		API_Vector3 GetGlobalScale();

		API_Vector3 GetForward();
		API_Vector3 GetRight();
		API_Vector3 GetUp();
		API_Vector3 GetBackward();
		API_Vector3 GetLeft();
		API_Vector3 GetDown();

		API_GameObject GetGameObject();

	private:
		void SetComponent(TransformComponent* transformComponent);
	private:
		TransformComponent* _transform = nullptr;

		friend class DragBoxTransform;
		friend class API_GameObject;
	};
}
