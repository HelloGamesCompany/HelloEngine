#include "Headers.h"
#include "PhysBody3D.h"
#include "ModulePhysics.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleLayers.h"
#include "PhysicsComponent.h"

PhysBody3D::PhysBody3D(btRigidBody* body)
{
	this->body = body;
	isRenderingCol = false;
	colShape = ColliderShape::NONE;
}

PhysBody3D::~PhysBody3D()
{
}

void PhysBody3D::Push(float x, float y, float z)
{
	if (body)
		body->applyCentralImpulse(btVector3(x, y, z));
}

void PhysBody3D::SetVelocity(float x, float y, float z)
{
	if (body)
		body->setLinearVelocity(btVector3(x, y, z));
}

void PhysBody3D::GetTransform(float* matrix) const
{
	if (body && matrix)
		body->getWorldTransform().getOpenGLMatrix(matrix);
}

void PhysBody3D::SetTransform(const float* matrix) const
{
	if (body && matrix)
	{
		btTransform t;
		t.setFromOpenGLMatrix(matrix);
		body->setWorldTransform(t);
	}
}

void PhysBody3D::SetPos(float x, float y, float z)
{
	if (!body)
		return;

	btTransform t = body->getWorldTransform();
	t.setOrigin(btVector3(x, y, z));
	body->setWorldTransform(t);
}

void PhysBody3D::SetRotation(float x, float y, float z)
{
	if (!body)
		return;

	btTransform t = body->getWorldTransform();
	btQuaternion quat;
	quat.setEulerZYX(z, y, x);
	t.setRotation(quat);
	body->setWorldTransform(t);
}

void PhysBody3D::SetScale(float x, float y, float z)
{
	btCollisionShape* shape = body->getCollisionShape();
	shape->setLocalScaling({ x, y, z });
	btVector3 inertia;
	shape->calculateLocalInertia(mass, inertia);
	body->setMassProps(mass, inertia);
}

float3 PhysBody3D::GetPos()
{
	btVector3 vec = body->getWorldTransform().getOrigin();

	return float3(vec.x(), vec.y(), vec.z());
}

float3 PhysBody3D::GetRotation()
{
	btVector3 vec;
	body->getWorldTransform().getBasis().getEulerZYX(vec[0], vec[1], vec[2]);

	return float3(vec.z(), vec.y(), vec.x());
}

float3 PhysBody3D::GetVelocity()
{
	btVector3 vec = body->getLinearVelocity();

	return float3(vec.x(), vec.y(), vec.z());
}


void PhysBody3D::Update()
{

	//btTransform worldTransform = body->getWorldTransform();
	//btScalar matrix;
	//worldTransform.getOpenGLMatrix(&matrix);
	
	// TODO: This check will not be necessary once we stop creating PhysBodies without game objects!!!
	if (ModuleLayers::gameObjects.count(gameObjectUID) != 0)
	{
		GameObject* go = ModuleLayers::gameObjects[gameObjectUID];
		go->transform->_ignorePhysBody = true; // This flag makes the PhysComponent ignore this Position update!
									 // The idea here is, if any other transformation occurs after this, it will be applied, but this one won't. Check PhysComponent::OnTransformCallback()
		go->transform->SetPosition(GetPos() - colPos);
		go->transform->_ignorePhysBody = true;
		go->transform->SetRotation(GetRotation());
	}
}

void PhysBody3D::SetShape(ColliderShape shape)
{
	colShape = shape;
}

void PhysBody3D::RenderCollider()

{
	if (isRenderingCol == true) {
		switch (colShape) {
		case ColliderShape::BOX:
			if (ModuleLayers::gameObjects.count(gameObjectUID) != 0)
			{
				GameObject* go = ModuleLayers::gameObjects[gameObjectUID];
				Application::Instance()->renderer3D->renderManager.DrawColliderBox(this, float4(go->GetComponent<PhysicsComponent>()->renderColColor), go->GetComponent<PhysicsComponent>()->wireframeSize);
			}
			
			break;
		case ColliderShape::SPHERE:
			// TODO: This check will not be necessary once we stop creating PhysBodies without game objects!!!
			if (ModuleLayers::gameObjects.count(gameObjectUID) != 0)
			{
				GameObject* go = ModuleLayers::gameObjects[gameObjectUID];
				Application::Instance()->renderer3D->renderManager.DrawColliderSphere(this, go->GetComponent<PhysicsComponent>()->sphereRadius, float4(go->GetComponent<PhysicsComponent>()->renderColColor), go->GetComponent<PhysicsComponent>()->wireframeSize, go->GetComponent<PhysicsComponent>()->sphereVerSlices, go->GetComponent<PhysicsComponent>()->sphereHorSlices);
			}
			
			break;
		case ColliderShape::CYLINDER:
			// TODO: This check will not be necessary once we stop creating PhysBodies without game objects!!!
			if (ModuleLayers::gameObjects.count(gameObjectUID) != 0)
			{
				GameObject* go = ModuleLayers::gameObjects[gameObjectUID];
				Application::Instance()->renderer3D->renderManager.DrawColliderCylinder(this, go->GetComponent<PhysicsComponent>()->cylRadiusHeight, float4(go->GetComponent<PhysicsComponent>()->renderColColor), go->GetComponent<PhysicsComponent>()->wireframeSize, go->GetComponent<PhysicsComponent>()->cylinderVerSlices);
			}
			
			break;
		}
		
	}

}

