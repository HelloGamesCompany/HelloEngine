#include "Headers.h"
#include "PhysBody3D.h"
#include "ModulePhysics.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleLayers.h"

PhysBody3D::PhysBody3D(btRigidBody* body)
{
	this->body = body;
	isRenderingCol = false;
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
	if (body) 
	{
		btTransform t = body->getWorldTransform();
		t.setOrigin(btVector3(x, y, z));
		body->setWorldTransform(t);
	}
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
			Application::Instance()->renderer3D->renderManager.DrawColliderBox(this);
			break;
		case ColliderShape::SPHERE:
			Application::Instance()->renderer3D->renderManager.DrawColliderSphere(this);
			break;
		case ColliderShape::CYLINDER:
			Application::Instance()->renderer3D->renderManager.DrawColliderCylinder(this);
			break;
		}
		
	}

}

