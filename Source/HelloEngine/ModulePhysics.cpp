#include "Headers.h"
#include "ModulePhysics.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "LayerGame.h"

btDiscreteDynamicsWorld* ModulePhysics::world = nullptr;

ModulePhysics::ModulePhysics()
{
	collision_conf = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collision_conf);
	broad_phase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	world = new btDiscreteDynamicsWorld(dispatcher, broad_phase, solver, collision_conf);
}

ModulePhysics::~ModulePhysics()
{
	S_RemovePhysBody(testBody);
	RELEASE(testBody);
	RELEASE(world);
	RELEASE(solver);
	RELEASE(broad_phase);
	RELEASE(dispatcher);
	RELEASE(collision_conf);
}

bool ModulePhysics::Init()
{
	return true;
}

bool ModulePhysics::Start()
{
	PrimCube cube = PrimCube(2);
	testBody = CreatePhysBody(&cube);

	return true;
}

UpdateStatus ModulePhysics::PreUpdate()
{
	if (LayerGame::S_IsPlaying())
	{
		world->stepSimulation(EngineTime::GameDeltaTime(), 15);
	}
	else
	{
		world->stepSimulation(0);
	}

	return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModulePhysics::Update()
{
	world->updateAabbs();
	return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModulePhysics::PostUpdate()
{
	return UpdateStatus::UPDATE_CONTINUE;
}

bool ModulePhysics::CleanUp()
{
	for (int i = world->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = world->getCollisionObjectArray()[i];
		world->removeCollisionObject(obj);
	}

	for (int i = 0; i < constraints.size(); i++)
	{
		world->removeConstraint(constraints[i]);
		RELEASE(constraints[i]);
	}

	constraints.clear();

	for (int i = 0; i < motions.size(); i++)
	{
		RELEASE(motions[i]);
	}

	motions.clear();

	for (int i = 0; i < shapes.size(); i++)
	{
		RELEASE(shapes[i]);
	}

	shapes.clear();

	//for (int i = 0; i < bodies.size(); i++)
	//{
	//	RELEASE(bodies[i]);
	//}

	//bodies.clear();

	return true;
}

PhysBody3D* ModulePhysics::CreatePhysBody(const Primitive* primitive, float mass)
{
	btCollisionShape* colShape = nullptr;
	btTransform setUpTransform;

	switch (primitive->GetType())
	{
	case PrimitiveTypes::Primitive_Cube:
	{
		PrimCube* cube = (PrimCube*)primitive;
		colShape = new btBoxShape(btVector3(cube->size.x * 0.5f, cube->size.y * 0.5f, cube->size.z * 0.5f));
		setUpTransform.setFromOpenGLMatrix(cube->transform.ptr());
	}
	break;
	case PrimitiveTypes::Primitive_Sphere:
	{
		PrimSphere* sphere = (PrimSphere*)primitive;
		colShape = new btSphereShape(sphere->radius);
		setUpTransform.setFromOpenGLMatrix(sphere->transform.ptr());
	}
	break;
	case PrimitiveTypes::Primitive_Cylinder:
	{
		PrimCylinder* cylinder = (PrimCylinder*)primitive;
		colShape = new btCylinderShapeX(btVector3(cylinder->height * 0.5f, cylinder->radius, 0.0f));
		setUpTransform.setFromOpenGLMatrix(cylinder->transform.ptr());
	}
	break;
	default:
		return nullptr;
	}

	shapes.push_back(colShape);

	btVector3 localInertia(0, 0, 0);

	if (mass != 0.f)
		colShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(setUpTransform);
	motions.push_back(myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	PhysBody3D* pbody = new PhysBody3D(body);

	body->setUserPointer(pbody);

	world->addRigidBody(body);

	//bodies.push_back(pbody);

	return pbody;
}

void ModulePhysics::S_RemovePhysBody(PhysBody3D* physBody)
{
	world->removeRigidBody(physBody->body);
	RELEASE(physBody->body);
}
