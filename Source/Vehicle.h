#ifndef __VEHICLE_H__
#define __VEHICLE_H__

#include "GameObject.h"
#include "Primitive.h"
#include "External/Bullet/include/btBulletDynamicsCommon.h"

class btRaycastVehicle;
class Smoke;

struct Wheel
{
	vec3 connection; // origin of the ray. Must come from within the chassis
	vec3 direction;
	vec3 axis;
	float suspensionRestLength; // max length for suspension in meters
	float radius;
	float width;
	bool front; // is front wheel ?
	bool drive; // does this wheel received engine power ?
	bool brake; // does breakes affect this wheel ?
	bool steering; // does this wheel turns ?
};

struct VehicleInfo
{
	~VehicleInfo();

	vec3 chassis_size;
	vec3 chassis_offset;

	vec3 test1_offset;
	vec3 test1_size;

	float mass;
	float suspensionStiffness; // default to 5.88 / 10.0 offroad / 50.0 sports car / 200.0 F1 car
	float suspensionCompression; // default to 0.83
	float suspensionDamping; // default to 0.88 / 0..1 0 bounces / 1 rigid / recommended 0.1...0.3
	float maxSuspensionTravelCm; // default to 500 cm suspension can be compressed
	float frictionSlip; // defaults to 10.5 / friction with the ground. 0.8 should be good but high values feels better (kart 1000.0)
	float maxSuspensionForce; // defaults to 6000 / max force to the chassis

	Wheel* wheels = nullptr;
	int num_wheels;
};

class Vehicle :  public GameObject
{
public:
	Vehicle(std::string name, Tag tag, Application* _app, btRaycastVehicle* vehicle, const VehicleInfo& info);
	Vehicle(std::string name, Tag tag, Application* _app);
	~Vehicle() override;

	void Start() override;
	void Update() override;
	void PostUpdate() override;
	void CleanUp() override;

	void OnCollisionEnter(PhysBody3D* col) override;

	void Respawn();

	vec3 GetObserverPos();
	void Render();
	void ApplyEngineForce(float force);
	void Brake(float force);
	void Turn(float degrees);
	float GetKmh() const;

	bool boostOn = false;

public:
	VehicleInfo* info = nullptr;
	btRaycastVehicle* vehicle = nullptr;

private:
	float turn = 3.0f;
	float rotateSpeed = 5.0f;
	float speed = 1000.0f;
	float acceleration = 0.0f;
	float maxVelocity = 180.0f;
	float nitroVelocity = 240.0f;
	float boostVelocity = 400.0f;

	float currentVelocity = maxVelocity;

	float boostCoolDown = 1.2f;
	float boostCounter = boostCoolDown;

	float frictionCoheficien = 0.3f;

	vec3 observerPos = { 0,0,0 };
	float observerDistance = 10.0f;
	float observerMaxDistance = 30.0f;
	float observerMinDistance = 10.0f;

	float smokeStep = 0.1f;
	float weelPrintStep = 0.2f;

	// Shapes
	Cylinder pipe_L;
	Cylinder pipe_R;

	// Fake Ground!!!
	Cube ground;

	uint nitrosfx,driftsfx;

  btTransform defaultTransform;
private:
	void InitShapes();

	void UpdateRotateLimit();

	void UpdateObserverDistance();

	void OrientWithCar(mat4x4& transform, btVector3 offset, float angle = 0);
};

#endif // !__VEHICLE_H__
