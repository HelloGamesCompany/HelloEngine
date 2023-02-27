#pragma once
#include "Module.h"
#include <vector>
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"


class Primitive;
class PhysBody3D;

class ModulePhysics : public Module
{
public:

	ModulePhysics(bool start_enabled = true);

	virtual ~ModulePhysics();

	bool Init() override;

	bool Start() override;

	UpdateStatus PreUpdate() override;

	UpdateStatus Update() override;

	UpdateStatus PostUpdate() override;

	bool CleanUp() override;

	PhysBody3D* CreatePhysBody(const Primitive* primitive, float mass = 1.0f);
	//PhysBody3D* CreatePhysBody();
	//PhysBody3D* CreatePhysBody();

private:

	btDiscreteDynamicsWorld* world = nullptr;

	bool debugDraw = false;

	//std::vector <btCollisionShape*> shapes;
	//std::vector <PhysBody3D*> bodies;
	//std::vector <btDefaultMotionState*> motions;
	//std::vector <btTypedConstraint*> constraints;
};