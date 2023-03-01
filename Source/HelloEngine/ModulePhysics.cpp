#include "Headers.h"
#include "ModulePhysics.h"
#include "Primitive.h"
#include "LayerGame.h"

ModulePhysics::ModulePhysics()
{
	//world = new btDiscreteDynamicsWorld(dispatcher, broad_phase, solver, collision_conf);
}

ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Init()
{
	return true;
}

bool ModulePhysics::Start()
{
	return true;
}

UpdateStatus ModulePhysics::PreUpdate()
{
	//world->stepSimulation(dt, 15);

	if (LayerGame::S_IsPlaying()) 
{
		//world->stepSimulation(EngineTime::GameDeltaTime(), 15);
	}
	else 
	{
		//world->stepSimulation(0);
	}

	return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModulePhysics::Update()
{
	//world->updateAabbs();
	return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModulePhysics::PostUpdate()
{
	return UpdateStatus::UPDATE_CONTINUE;
}

bool ModulePhysics::CleanUp()
{
	return false;
}

PhysBody3D* ModulePhysics::CreatePhysBody(const Primitive* primitive, float mass)
{
	btCollisionShape* colShape = nullptr;
	btTransform startTransform;

	switch (primitive->GetType())
	{
	case PrimitiveTypes::Primitive_Cube: 
	{
		PrimCube* cube = (PrimCube*)primitive;
		colShape = new btBoxShape(btVector3(cube->size.x * 0.5f, cube->size.y * 0.5f, cube->size.z * 0.5f));

		startTransform.setFromOpenGLMatrix(cube->transform.ptr());
	}
	break;
	case PrimitiveTypes::Primitive_Sphere:
	{
		PrimSphere* sphere = (PrimSphere*)primitive;
		colShape = new btSphereShape(sphere->radius);
		startTransform.setFromOpenGLMatrix(sphere->transform.ptr());
	}
	break;
	case PrimitiveTypes::Primitive_Cylinder:
	{
		PrimCylinder* cylinder = (PrimCylinder*)primitive;
		colShape = new btCylinderShapeX(btVector3(cylinder->height * 0.5f, cylinder->radius, 0.0f));
		startTransform.setFromOpenGLMatrix(cylinder->transform.ptr());
	}
	break;
	default:
		return nullptr;
	}
	//shapes.push_back(colShape);

	btVector3 localInertia(0, 0, 0);
	if (mass != 0.f)
		colShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	//motions.push_back(myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	//PhysBody3D* pbody = new PhysBody3D(body);

	//body->setUserPointer(pbody);
	world->addRigidBody(body);

	//bodies.push_back(pbody);

	return nullptr;
}

void ModulePhysics::RemovePhysBody(PhysBody3D* physBody)
{
	//world->removeRigidBody(physBody->body);
}
