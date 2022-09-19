#ifndef __MODULEPHYSICS3D_H__
#define __MODULEPHYSICS3D_H__

#include "Module.h"
#include "List.h"
#include "Primitive.h"
#include "PhysBody3D.h"

#include "External/Bullet/include/btBulletDynamicsCommon.h"

// Recommended scale is 1.0f == 1 meter, no less than 0.2 objects
#define GRAVITY btVector3(0.0f, -10.0f, 0.0f) 

class DebugDrawer;
class Vehicle;
struct VehicleInfo;

class ModulePhysics3D : public Module
{
public:
	ModulePhysics3D(Application* app, bool start_enabled = true);
	~ModulePhysics3D();

	bool Init();
	bool Start();
	UpdateStatus PreUpdate();
	UpdateStatus Update();
	UpdateStatus PostUpdate();
	bool CleanUp();

	PhysBody3D* AddBody(const Sphere& sphere, float mass = 1.0f);
	PhysBody3D* AddBody(const Cube& cube, float mass = 1.0f);
	PhysBody3D* AddBody(const Cylinder& cylinder, float mass = 1.0f);
	PhysBody3D* AddTriggerArea(const Cylinder& cylinder, vec3 pos);
	PhysBody3D* AddTriggerCube(const Cube& cube, vec3 pos);
	/*Vehicle* AddVehicle(const VehicleInfo& info);*/

	PhysBody3D* CreateCube(float x, float y, float z, float mass);
	PhysBody3D* CreateCylinder(float radius, float height, float mass);
	PhysBody3D* CreateCylinderArea(float radius, float height, vec3 pos);
	PhysBody3D* CreateCubeArea(float x, float y, float z, vec3 pos);

	void AddConstraintP2P(PhysBody3D& bodyA, PhysBody3D& bodyB, const vec3& anchorA, const vec3& anchorB);
	void AddConstraintHinge(PhysBody3D& bodyA, PhysBody3D& bodyB, const vec3& anchorA, const vec3& anchorB, const vec3& axisS, const vec3& axisB, bool disable_collision = false);

private:

	bool debug = false;

	btDefaultCollisionConfiguration*	collision_conf = nullptr;
	btCollisionDispatcher*				dispatcher = nullptr;
	btBroadphaseInterface*				broad_phase = nullptr;
	btSequentialImpulseConstraintSolver* solver = nullptr;
	btDiscreteDynamicsWorld*			world = nullptr;
	btDefaultVehicleRaycaster*			vehicle_raycaster = nullptr;
	DebugDrawer*						debug_draw = nullptr;

	List<btCollisionShape*> shapes;
	List<PhysBody3D*> bodies;
	List<btDefaultMotionState*> motions;
	List<btTypedConstraint*> constraints;
	//List<Vehicle*> vehicles;

	friend class Vehicle;
};

class DebugDrawer : public btIDebugDraw
{
public:
	DebugDrawer() : line(0,0,0)
	{}

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	void reportErrorWarning(const char* warningString);
	void draw3dText(const btVector3& location, const char* textString);
	void setDebugMode(int debugMode);
	int	 getDebugMode() const;

	DebugDrawModes mode;
	Line line;
	Primitive point;
};

#endif // !__MODULEPHYSICS3D_H__