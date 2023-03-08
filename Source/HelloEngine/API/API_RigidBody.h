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
