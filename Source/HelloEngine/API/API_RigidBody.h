#pragma once

#include "Globals.h"
#include "API_GameObject.h"

class PhysicsComponent;

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

	private:
		PhysicsComponent* GetComponent();
		void SetComponent(PhysicsComponent* component);
	private:
		PhysicsComponent* _rigidBody = nullptr;

		friend class API_GameObject;
	};
}
