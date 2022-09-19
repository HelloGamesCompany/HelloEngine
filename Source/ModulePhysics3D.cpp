#include "ModulePhysics3D.h"
#include "ModuleInput.h"
#include "Vehicle.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"
#include <iostream>

ModulePhysics3D::ModulePhysics3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug = false;

	collision_conf = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collision_conf);
	broad_phase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	debug_draw = new DebugDrawer();
}

// Destructor
ModulePhysics3D::~ModulePhysics3D()
{
	RELEASE(debug_draw);
	RELEASE(solver);
	RELEASE(broad_phase);
	RELEASE(dispatcher);
	RELEASE(collision_conf);
}

// Render not available yet----------------------------------
bool ModulePhysics3D::Init()
{
	LOG("Creating 3D Physics simulation");
	bool ret = true;

	return ret;
}

// ---------------------------------------------------------
bool ModulePhysics3D::Start()
{
	LOG("Creating Physics environment");

	world = new btDiscreteDynamicsWorld(dispatcher, broad_phase, solver, collision_conf);
	world->setDebugDrawer(debug_draw);
	world->setGravity(GRAVITY);
	vehicle_raycaster = new btDefaultVehicleRaycaster(world);

	// Big plane as ground	
	btCollisionShape* colShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	shapes.add(colShape);

	btDefaultMotionState* myMotionState = new btDefaultMotionState();
	motions.add(myMotionState);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, myMotionState, colShape);

	btRigidBody* body = new btRigidBody(rbInfo);

	body->setFriction(1);

	world->addRigidBody(body);
	
	return true;
}

// ---------------------------------------------------------
UpdateStatus ModulePhysics3D::PreUpdate()
{
	world->stepSimulation(App->fps , 15);

	int numManifolds = world->getDispatcher()->getNumManifolds();
	for(int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA = (btCollisionObject*)(contactManifold->getBody0());
		btCollisionObject* obB = (btCollisionObject*)(contactManifold->getBody1());

		int numContacts = contactManifold->getNumContacts();
		if(numContacts > 0)
		{
			PhysBody3D* pBodyA = (PhysBody3D*)obA->getUserPointer();
			PhysBody3D* pBodyB = (PhysBody3D*)obB->getUserPointer();

			if(pBodyA && pBodyB)
			{
				if (pBodyA->gameObject != nullptr)
					pBodyA->gameObject->OnCollisionEnter(pBodyB);

				if (pBodyB->gameObject != nullptr)
					pBodyB->gameObject->OnCollisionEnter(pBodyA);

				if (pBodyA->gameObject != nullptr)
					if(pBodyA->gameObject->tag == Tag::TaxiClient)
						pBodyA->gameObject->OnTriggerEnter(pBodyB);

				if (pBodyB->gameObject != nullptr)
					if(pBodyB->gameObject->tag == Tag::TaxiClient)
						pBodyB->gameObject->OnTriggerEnter(pBodyA);
			}
		}
	}

	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
UpdateStatus ModulePhysics3D::Update()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(debug == true)
	{
		world->debugDrawWorld();

		if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			Sphere s(1);
			s.SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);
			float force = 30.0f;
			AddBody(s)->Push(-(App->camera->Z.x * force), -(App->camera->Z.y * force), -(App->camera->Z.z * force));
		}
	}

	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
UpdateStatus ModulePhysics3D::PostUpdate()
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModulePhysics3D::CleanUp()
{
	LOG("Destroying 3D Physics simulation");

	// Remove from the world all collision bodies
	for(int i = world->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = world->getCollisionObjectArray()[i];
		world->removeCollisionObject(obj);
		RELEASE(obj);
	}

	for (int i = 0, count = constraints.count(); i < count; i++)
	{
		world->removeConstraint(constraints[i]);
		RELEASE(constraints[i]);
	}

	constraints.clear();

	for (int i = 0, count = motions.count(); i < count; i++)
	{
		RELEASE(motions[i]);
	}

	motions.clear();

	for (int i = 0, count = shapes.count(); i < count; i++)
	{
		RELEASE(shapes[i]);
	}

	shapes.clear();

	for (int i = 0, count = bodies.count(); i < count; i++)
	{
		RELEASE(bodies[i]);
	}

	bodies.clear();

	//for (int i = 0, count = vehicles.count(); i < count; i++)
	//{
	//	RELEASE(vehicles[i]);
	//}

	//vehicles.clear();

	RELEASE(vehicle_raycaster);
	RELEASE(world);

	return true;
}

// ---------------------------------------------------------
PhysBody3D* ModulePhysics3D::AddBody(const Sphere& sphere, float mass)
{
	btCollisionShape* colShape = new btSphereShape(sphere.radius);
	shapes.add(colShape);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(&sphere.transform);

	btVector3 localInertia(0, 0, 0);
	if(mass != 0.f)
		colShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	motions.add(myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	PhysBody3D* pbody = new PhysBody3D(body);

	body->setUserPointer(pbody);
	world->addRigidBody(body);
	bodies.add(pbody);

	return pbody;
}

// ---------------------------------------------------------
PhysBody3D* ModulePhysics3D::AddBody(const Cube& cube, float mass)
{
	btCollisionShape* colShape = new btBoxShape(btVector3(cube.size.x*0.5f, cube.size.y*0.5f, cube.size.z*0.5f));
	shapes.add(colShape);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(&cube.transform);

	btVector3 localInertia(0, 0, 0);
	if(mass != 0.f)
		colShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	motions.add(myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	PhysBody3D* pbody = new PhysBody3D(body);

	body->setUserPointer(pbody);
	world->addRigidBody(body);
	bodies.add(pbody);

	return pbody;
}

// ---------------------------------------------------------
PhysBody3D* ModulePhysics3D::AddBody(const Cylinder& cylinder, float mass)
{
	btCollisionShape* colShape = new btCylinderShapeX(btVector3(cylinder.height*0.5f, cylinder.radius, 0.0f));
	shapes.add(colShape);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(&cylinder.transform);

	btVector3 localInertia(0, 0, 0);
	if(mass != 0.f)
		colShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	motions.add(myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	PhysBody3D* pbody = new PhysBody3D(body);

	body->setUserPointer(pbody);
	world->addRigidBody(body);
	bodies.add(pbody);

	return pbody;
}

// ---------------------------------------------------------
PhysBody3D* ModulePhysics3D::AddTriggerArea(const Cylinder& cylinder, vec3 pos)
{
	btCollisionShape* colShape = new btCylinderShapeX(btVector3(cylinder.height * 0.5f, cylinder.radius, 0.0f));
	shapes.add(colShape);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(&cylinder.transform);

	btVector3 localInertia(0, 0, 0);
	colShape->calculateLocalInertia(0.f, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	motions.add(myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(0.f, myMotionState, colShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);

	body->setCollisionFlags(btCollisionObject::CO_GHOST_OBJECT); //Collision without physics callbacks
	//body.setCollision

	PhysBody3D* pbody = new PhysBody3D(body);
	pbody->SetPos(pos.x, pos.y, pos.z);

	body->setUserPointer(pbody);
	world->addRigidBody(body);
	bodies.add(pbody);

	return pbody;
}

PhysBody3D* ModulePhysics3D::AddTriggerCube(const Cube& cube, vec3 pos)
{
	btCollisionShape* colShape = new btBoxShape(btVector3(cube.size.x * 0.5f, cube.size.y * 0.5f, cube.size.z * 0.5f));
	shapes.add(colShape);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(&cube.transform);

	btVector3 localInertia(0, 0, 0);
	colShape->calculateLocalInertia(0.f, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	motions.add(myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(0.f, myMotionState, colShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);

	body->setCollisionFlags(btCollisionObject::CO_GHOST_OBJECT); //Collision without physics callbacks
	//body.setCollision

	PhysBody3D* pbody = new PhysBody3D(body);
	pbody->SetPos(pos.x, pos.y, pos.z);

	body->setUserPointer(pbody);
	world->addRigidBody(body);
	bodies.add(pbody);

	return pbody;
}
// ---------------------------------------------------------
//Vehicle* ModulePhysics3D::AddVehicle(const VehicleInfo& info)
//{
//	btCompoundShape* comShape = new btCompoundShape();
//	shapes.add(comShape);
//
//	btCollisionShape* colShape = new btBoxShape(btVector3(info.chassis_size.x*0.5f, info.chassis_size.y*0.5f, info.chassis_size.z*0.5f));
//	shapes.add(colShape);
//
//	btTransform trans;
//	trans.setIdentity();
//	trans.setOrigin(btVector3(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z));
//
//	comShape->addChildShape(trans, colShape);
//
//	btTransform startTransform;
//	startTransform.setIdentity();
//
//	btVector3 localInertia(0, 0, 0);
//	comShape->calculateLocalInertia(info.mass, localInertia);
//
//	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
//	btRigidBody::btRigidBodyConstructionInfo rbInfo(info.mass, myMotionState, comShape, localInertia);
//
//	btRigidBody* body = new btRigidBody(rbInfo);
//	body->setContactProcessingThreshold(BT_LARGE_FLOAT);
//	body->setActivationState(DISABLE_DEACTIVATION);
//
//	world->addRigidBody(body);
//
//	btRaycastVehicle::btVehicleTuning tuning;
//	tuning.m_frictionSlip = info.frictionSlip;
//	tuning.m_maxSuspensionForce = info.maxSuspensionForce;
//	tuning.m_maxSuspensionTravelCm = info.maxSuspensionTravelCm;
//	tuning.m_suspensionCompression = info.suspensionCompression;
//	tuning.m_suspensionDamping = info.suspensionDamping;
//	tuning.m_suspensionStiffness = info.suspensionStiffness;
//
//	btRaycastVehicle* vehicle = new btRaycastVehicle(tuning, body, vehicle_raycaster);
//
//	vehicle->setCoordinateSystem(0, 1, 2);
//
//	for(int i = 0; i < info.num_wheels; ++i)
//	{
//		btVector3 conn(info.wheels[i].connection.x, info.wheels[i].connection.y, info.wheels[i].connection.z);
//		btVector3 dir(info.wheels[i].direction.x, info.wheels[i].direction.y, info.wheels[i].direction.z);
//		btVector3 axis(info.wheels[i].axis.x, info.wheels[i].axis.y, info.wheels[i].axis.z);
//
//		vehicle->addWheel(conn, dir, axis, info.wheels[i].suspensionRestLength, info.wheels[i].radius, tuning, info.wheels[i].front);
//	}
//	// ---------------------
//
//	Vehicle* pvehicle = new Vehicle(body, vehicle, info);
//	world->addVehicle(vehicle);
//	vehicles.add(pvehicle);
//
//	return pvehicle;
//}

PhysBody3D* ModulePhysics3D::CreateCube(float x, float y, float z, float mass)
{
	Cube cube;
	cube.size.x = x;
	cube.size.y = y;
	cube.size.z = z;

	return AddBody(cube, mass);
}

PhysBody3D* ModulePhysics3D::CreateCylinder(float radius, float height, float mass)
{
	Cylinder cylinder;
	cylinder.radius = radius;
	cylinder.height = height;
	cylinder.SetRotation(90, {0, 0, 1});

	return AddBody(cylinder, mass);
}

PhysBody3D* ModulePhysics3D::CreateCylinderArea(float radius, float height, vec3 pos)
{
	Cylinder cylinder;
	cylinder.radius = radius;
	cylinder.height = height;
	cylinder.SetRotation(90, { 0, 0, 1 });

	return AddTriggerArea(cylinder, pos);
}

PhysBody3D* ModulePhysics3D::CreateCubeArea(float x, float y, float z, vec3 pos)
{
	Cube cube;
	cube.size.x = x;
	cube.size.y = y;
	cube.size.z = z;

	return AddTriggerCube(cube, pos);
}

// ---------------------------------------------------------
void ModulePhysics3D::AddConstraintP2P(PhysBody3D& bodyA, PhysBody3D& bodyB, const vec3& anchorA, const vec3& anchorB)
{
	btTypedConstraint* p2p = new btPoint2PointConstraint(
		*(bodyA.body), 
		*(bodyB.body), 
		btVector3(anchorA.x, anchorA.y, anchorA.z), 
		btVector3(anchorB.x, anchorB.y, anchorB.z));
	p2p->enableFeedback(true);
	world->addConstraint(p2p);
	constraints.add(p2p);
	p2p->setDbgDrawSize(2.0f);
}

void ModulePhysics3D::AddConstraintHinge(PhysBody3D& bodyA, PhysBody3D& bodyB, const vec3& anchorA, const vec3& anchorB, const vec3& axisA, const vec3& axisB, bool disable_collision)
{
	btHingeConstraint* hinge = new btHingeConstraint(
		*(bodyA.body), 
		*(bodyB.body), 
		btVector3(anchorA.x, anchorA.y, anchorA.z),
		btVector3(anchorB.x, anchorB.y, anchorB.z),
		btVector3(axisA.x, axisA.y, axisA.z), 
		btVector3(axisB.x, axisB.y, axisB.z));

	world->addConstraint(hinge, disable_collision);
	constraints.add(hinge);
	hinge->setDbgDrawSize(2.0f);
}

// =============================================
void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	line.origin.Set(from.getX(), from.getY(), from.getZ());
	line.destination.Set(to.getX(), to.getY(), to.getZ());
	line.color.Set(color.getX(), color.getY(), color.getZ());
	line.Render();
}

void DebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	point.transform.translate(PointOnB.getX(), PointOnB.getY(), PointOnB.getZ());
	point.color.Set(color.getX(), color.getY(), color.getZ());
	point.Render();
}

void DebugDrawer::reportErrorWarning(const char* warningString)
{
	LOG("Bullet warning: %s", warningString);
}

void DebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
	LOG("Bullet draw text: %s", textString);
}

void DebugDrawer::setDebugMode(int debugMode)
{
	mode = (DebugDrawModes) debugMode;
}

int	 DebugDrawer::getDebugMode() const
{
	return mode;
}
