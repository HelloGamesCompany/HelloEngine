#include "Headers.h"
#include "ModulePhysics.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "LayerGame.h"
#include "ModuleLayers.h"

btDiscreteDynamicsWorld* ModulePhysics::world = nullptr;
std::vector <PhysBody3D*> ModulePhysics::physBodies;

ModulePhysics::ModulePhysics()
{
	collision_conf = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collision_conf);
	broad_phase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
}

ModulePhysics::~ModulePhysics()
{
	//S_RemovePhysBody(testBody);
	//RELEASE(testBody);
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
	world = new btDiscreteDynamicsWorld(dispatcher, broad_phase, solver, collision_conf);
	world->setGravity(btVector3(0.0f, -10.0f, 0.0f));

	// Testing------------------------------------------
	PrimCube cube = PrimCube(2);
	///////testBody = CreatePhysBody(&cube);
	//testBody->SetVelocity(12, 10, 10);
	///////testBody->SetPos(10, 10, 10);
	//PrimCube cube = PrimCube(2);
	//testBody = CreatePhysBody(&cube);
	//testBody2 = CreatePhysBody(&cube);
	////testBody->SetVelocity(12, 10, 10);
	//testBody->SetPos(10, 0, 0);
	//testBody2->SetPos(-10, 0, 0);
	//testBody->SetVelocity(-0.05f, 0, 0);
	//testBody2->SetVelocity(0.05f, 0, 0);

	//testBody->body->setGravity(btVector3(0, 0, 0));
	//testBody2->body->setGravity(btVector3(0, 0, 0));
	// Testing------------------------------------------

	return true;
}

UpdateStatus ModulePhysics::PreUpdate()
{
	//world->stepSimulation(Application::Instance()->GetDeltaTime(), 15);

	////testBody->Push(1, 1, 1);
	//testBody->body->activate(true);
	////std::cout << "\ndt:" << EngineTime::GameDeltaTime()<<std::endl;
	////std::cout << "\n-------------------------\nx" << testBody->GetVelocity().x << "\ny" << testBody->GetVelocity().y << "\nz" << testBody->GetVelocity().z;
	//std::cout <<"\n-------------------------\nx" << testBody->GetPos().x << "\ny" << testBody->GetPos().y << "\nz" << testBody->GetPos().z;
	if (LayerGame::S_IsPlaying())
	{
		world->stepSimulation(EngineTime::GameDeltaTime(), 15);
	}
	else
	{
		world->stepSimulation(0);
	}

	//std::cout << "\n-------------------------\nx" << testBody->GetPos().x << "\ny" << testBody->GetPos().y << "\nz" << testBody->GetPos().z;
	//std::cout << "\n-------------------------\nx" << testBody2->GetPos().x << "\ny" << testBody2->GetPos().y << "\nz" << testBody2->GetPos().z;
	int numManifolds = world->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA = (btCollisionObject*)(contactManifold->getBody0());
		btCollisionObject* obB = (btCollisionObject*)(contactManifold->getBody1());

		int numContacts = contactManifold->getNumContacts();
		if (numContacts > 0)
		{
			PhysBody3D* pBodyA = (PhysBody3D*)obA->getUserPointer();
			PhysBody3D* pBodyB = (PhysBody3D*)obB->getUserPointer();

			if (pBodyA && pBodyB)
			{
				if (ModuleLayers::gameObjects.count(pBodyA->gameObjectUID) != 0 && ModuleLayers::gameObjects[pBodyA->gameObjectUID] != nullptr)
					ModuleLayers::gameObjects[pBodyA->gameObjectUID]->OnCollisionEnter(pBodyB);

				if (ModuleLayers::gameObjects.count(pBodyB->gameObjectUID) != 0 && ModuleLayers::gameObjects[pBodyB->gameObjectUID] != nullptr)
					ModuleLayers::gameObjects[pBodyB->gameObjectUID]->OnCollisionEnter(pBodyA);

			}
		}
	}

	return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModulePhysics::Update()
{
	world->updateAabbs();

	for (int i = 0; i < physBodies.size(); i++) {

		//physBodies[i]->RenderCollider();

		if (LayerGame::S_IsPlaying())
		{
			physBodies[i]->Update();
		}
	}

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

	body->setActivationState(DISABLE_DEACTIVATION);

	PhysBody3D* pbody = new PhysBody3D(body);

	switch (primitive->GetType())
	{
		case PrimitiveTypes::Primitive_Cube:
		{
			pbody->SetShape(ColliderShape::BOX);
		}
		break;
		case PrimitiveTypes::Primitive_Sphere:
		{
			pbody->SetShape(ColliderShape::SPHERE);
		}
		break;
		case PrimitiveTypes::Primitive_Cylinder:
		{
			pbody->SetShape(ColliderShape::CYLINDER);
		}
		break;
	}

	body->setUserPointer(pbody);

	world->addRigidBody(body);

	physBodies.push_back(pbody);

	return pbody;
}

void ModulePhysics::S_RemovePhysBody(PhysBody3D* physBody)
{
	world->removeRigidBody(physBody->body);
	RELEASE(physBody->body);
	for (int i = 0; i < physBodies.size(); ++i)
	{
		if (physBodies[i] == physBody)
		{
			RELEASE(physBodies[i]);
			physBodies.erase(physBodies.begin() + i);
		}
	}
}

void ModulePhysics::UpdatePhysBodyPos(PhysBody3D* physBody, float3 posVec)
{
	float3 currentTransformPos = ModuleLayers::gameObjects[physBody->gameObjectUID]->transform->GetGlobalPosition();
	physBody->SetPos(posVec.x + currentTransformPos.x, posVec.y + currentTransformPos.y, posVec.z + currentTransformPos.z);
}

void ModulePhysics::UpdatePhysBodyPos(PhysBody3D* physBody, float x, float y, float z)
{
	physBody->SetPos(x, y, z);
}
