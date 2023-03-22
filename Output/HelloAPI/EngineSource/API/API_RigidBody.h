#pragma once

#include "Globals.h"
#include "API/API_GameObject.h"
#include "API/API_Vector3.h"

class PhysicsComponent;
class ScriptComponent;

namespace API
{
	class TO_API API_RigidBody
	{
	public:
		API_RigidBody();
		~API_RigidBody();

		API_GameObject GetGameObject();

		void SetVelocity(API_Vector3 vec);
		API_Vector3 GetVelocity();

		void SetGravity(API_Vector3 gravity);
		API_Vector3 GetGravity();

		/// <summary>
		/// If the rigidbody is not a Sphere, there will return 0
		/// </summary>
		/// <returns></returns>
		float GetRadius();

	private:
		PhysicsComponent* GetComponent();
		void SetComponent(PhysicsComponent* component);
	private:
		PhysicsComponent* _rigidBody = nullptr;

		friend class API_GameObject;
		friend class ScriptComponent;
		friend class DragBoxRigidBody;
	};
}
