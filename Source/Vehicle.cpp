#include "Vehicle.h"
#include "ModulePhysics3D.h"
#include "PhysBody3D.h"
#include "ModuleScene.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "Smoke.h"
#include "WeelPrint.h"
#include <iostream>

Vehicle::Vehicle(std::string name, Tag tag, Application* _app, btRaycastVehicle* vehicle, const VehicleInfo& info) :GameObject(name, tag, _app)
{
}

Vehicle::Vehicle(std::string name, Tag tag, Application* _app):GameObject(name, tag, _app)
{
}

Vehicle::~Vehicle()
{
	RELEASE(vehicle);
	RELEASE(info);
}

VehicleInfo::~VehicleInfo()
{
	RELEASE_ARRAY(wheels);
}

void Vehicle::Start()
{
	// Shapes
	InitShapes();

	// Camera relative pos
	btVector3 v = { 0, 6, -20 };
	v.normalize();
	observerPos.Set(v.x(), v.y(), v.z());
	observerDistance = 21;

	info = new VehicleInfo();
	// Car properties ----------------------------------------
	info->chassis_size.Set(3, 1.2f, 4);
	info->chassis_offset.Set(0, 0.6f, -0.1f);

	info->mass = 1000.0f;
	info->suspensionStiffness = 15.88f;
	info->suspensionCompression = 0.83f;
	info->suspensionDamping = 0.88f;
	info->maxSuspensionTravelCm = 1000.0f;
	info->frictionSlip = 200.5f;
	info->maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------
	float half_width = info->chassis_size.x * 0.5f;
	float half_length = info->chassis_size.z * 0.5f;

	vec3 direction(0, -1, 0);
	vec3 axis(-1, 0, 0);

	info->num_wheels = 4;
	info->wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	info->wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	info->wheels[0].direction = direction;
	info->wheels[0].axis = axis;
	info->wheels[0].suspensionRestLength = suspensionRestLength;
	info->wheels[0].radius = wheel_radius;
	info->wheels[0].width = wheel_width;
	info->wheels[0].front = true;
	info->wheels[0].drive = true;
	info->wheels[0].brake = false;
	info->wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	info->wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	info->wheels[1].direction = direction;
	info->wheels[1].axis = axis;
	info->wheels[1].suspensionRestLength = suspensionRestLength;
	info->wheels[1].radius = wheel_radius;
	info->wheels[1].width = wheel_width;
	info->wheels[1].front = true;
	info->wheels[1].drive = true;
	info->wheels[1].brake = false;
	info->wheels[1].steering = true;

	// REAR-LEFT ------------------------
	info->wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	info->wheels[2].direction = direction;
	info->wheels[2].axis = axis;
	info->wheels[2].suspensionRestLength = suspensionRestLength;
	info->wheels[2].radius = wheel_radius;
	info->wheels[2].width = wheel_width;
	info->wheels[2].front = false;
	info->wheels[2].drive = false;
	info->wheels[2].brake = true;
	info->wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	info->wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	info->wheels[3].direction = direction;
	info->wheels[3].axis = axis;
	info->wheels[3].suspensionRestLength = suspensionRestLength;
	info->wheels[3].radius = wheel_radius;
	info->wheels[3].width = wheel_width;
	info->wheels[3].front = false;
	info->wheels[3].drive = false;
	info->wheels[3].brake = true;
	info->wheels[3].steering = false;

	btCompoundShape* comShape = new btCompoundShape();
	_app->physics->shapes.add(comShape);

	// Collision shape
	btCollisionShape* colShape = new btBoxShape(btVector3(info->chassis_size.x * 0.5f, info->chassis_size.y * 0.5f, info->chassis_size.z * 0.5f));
	_app->physics->shapes.add(colShape);

	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(btVector3(info->chassis_offset.x, info->chassis_offset.y, info->chassis_offset.z));

	comShape->addChildShape(trans, colShape);
	// Collision shape

	btTransform startTransform;
	startTransform.setIdentity();

	btVector3 localInertia(0, 0, 0);
	comShape->calculateLocalInertia(info->mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	_app->physics->motions.add(myMotionState);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(info->mass, myMotionState, comShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	body->setContactProcessingThreshold(BT_LARGE_FLOAT);
	body->setActivationState(DISABLE_DEACTIVATION);
	body->setAngularFactor(btVector3(0, 1, 0));

	body->setRestitution(0.8f);

	_app->physics->world->addRigidBody(body);

	btRaycastVehicle::btVehicleTuning tuning;
	tuning.m_frictionSlip = info->frictionSlip;
	tuning.m_maxSuspensionForce = info->maxSuspensionForce;
	tuning.m_maxSuspensionTravelCm = info->maxSuspensionTravelCm;
	tuning.m_suspensionCompression = info->suspensionCompression;
	tuning.m_suspensionDamping = info->suspensionDamping;
	tuning.m_suspensionStiffness = info->suspensionStiffness;

	vehicle = new btRaycastVehicle(tuning, body, _app->physics->vehicle_raycaster);

	vehicle->setCoordinateSystem(0, 1, 2);

	for (int i = 0; i < info->num_wheels; ++i)
	{
		btVector3 conn(info->wheels[i].connection.x, info->wheels[i].connection.y, info->wheels[i].connection.z);
		btVector3 dir(info->wheels[i].direction.x, info->wheels[i].direction.y, info->wheels[i].direction.z);
		btVector3 axis(info->wheels[i].axis.x, info->wheels[i].axis.y, info->wheels[i].axis.z);

		vehicle->addWheel(conn, dir, axis, info->wheels[i].suspensionRestLength, info->wheels[i].radius, tuning, info->wheels[i].front);	
	}

	pBody = new PhysBody3D(body, this);

	body->setUserPointer(pBody);
	pBody->SetPos(150, 5, 180);

	defaultTransform = body->getWorldTransform();

	_app->physics->bodies.add(pBody);

	_app->physics->world->addVehicle(vehicle);

	driftsfx = _app->audio->LoadFx("Assets/Audios/Sfx/nitrosfx.wav", 10);
	nitrosfx = _app->audio->LoadFx("Assets/Audios/Sfx/driftsfx.wav", 10);
}

void Vehicle::Update()
{
	if(_app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		Respawn();
	}

	vehicle->updateVehicle(_app->fps);

	//printf("X:%f\t Y:%f\t Z:%f\t\n", GetPosition().x, GetPosition().y, GetPosition().z);

	smokeStep -= _app->fps;
	weelPrintStep -= _app->fps;

	// Update countdown
	if (boostCounter > 0.0f) boostCounter -= _app->fps;
	else
	{
		boostOn = false;
		acceleration = 0;
	}
	// Brake
	if (_app->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
	{
		if (!boostOn)Brake(150);
		else Brake(350);		
		_app->audio->PlayFx(driftsfx, 0);

		float currentSpeed = GetKmh();

		if (abs(currentSpeed) < 250)
		{
			turn = 4.0f;
		}
		else
		{
			turn = 2.5f;
		}

		if (smokeStep <= 0)
		{
			// Calculate pos & rot
			// smoke 1
			mat4x4 tempMatrix;
			OrientWithCar(tempMatrix, {-1,0,-3.2f});
			vec3 pos = vec3(tempMatrix.M[12], tempMatrix.M[13], tempMatrix.M[14]);

			// smoke 2
			mat4x4 tempMatrix2;
			OrientWithCar(tempMatrix2, { 1,0,-3.2f });
			vec3 pos2 = vec3(tempMatrix2.M[12], tempMatrix2.M[13], tempMatrix2.M[14]);

			float lifeTime = ((rand() % 10) + 5) / 10;

			new Smoke(_app, lifeTime, pos);

			new Smoke(_app, lifeTime, pos2);

			smokeStep = 0.05;
		}
		
		
	}
	else if (_app->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_UP)
	{
		Brake(0);
		turn = 3.0f;
		Mix_HaltChannel(-1);
	}
	// Accelerate
	else if (_app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	{
		if (!boostOn) acceleration = 4000.0f;
		currentVelocity = nitroVelocity;
		if (Mix_Playing(2) == 0)
		{
			_app->audio->PlayFx(nitrosfx, 0, 2);
		}
		if (smokeStep <= 0);
		{
			// Calculate pos & rot
			// fire 1 pos
			mat4x4 tempMatrix;
			OrientWithCar(tempMatrix, { 1, 0, -3 });
			vec3 pos = vec3(tempMatrix.M[12], tempMatrix.M[13], tempMatrix.M[14]);

			// fire 2 pos
			mat4x4 tempMatrix2;
			OrientWithCar(tempMatrix2, { -1, 0, -3 });
			vec3 pos2 = vec3(tempMatrix2.M[12], tempMatrix2.M[13], tempMatrix2.M[14]);
			
			// Create fires
			float lifeTime = ((rand() % 10) + 5) / 10;

			int randColor = rand() % 2;

			Color color = Color(1.0f, 0.0f, 0.0f);

			color.g = ((float)(rand() % 100) + 1) / 100.0f;

			new Smoke(_app, lifeTime, pos, color);

			new Smoke(_app, lifeTime, pos2, color);

			smokeStep = 0.1f;
		}
	}
	else if (_app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_UP)
	{
		if (!boostOn) acceleration = 0.0f;
		currentVelocity = maxVelocity;
		Mix_HaltChannel(2);
	}
	
	if (boostOn && boostCounter > 0.0f)
	{
		acceleration = 6000;
		currentVelocity = boostVelocity;
		
	}

	// Go ahead
	if (_app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		if (GetKmh() < currentVelocity)
		{
			ApplyEngineForce(speed + acceleration);
		}
		else
		{
			ApplyEngineForce(0);
		}
	}
	// Back
	if(_app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		if (GetKmh() > -currentVelocity)
		{
			ApplyEngineForce(-speed);
		}		
		else
		{
			ApplyEngineForce(0);
		}
	}
	// Stop add force
	if (_app->input->GetKey(SDL_SCANCODE_UP) == KEY_UP || _app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	{
		ApplyEngineForce(0);

		// Fake friction
		Brake(GetKmh()* frictionCoheficien);
	}

	// Rotate
	if (_app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		// Apply rotation
		Turn(-turn * DEGTORAD);
	}
	else if (_app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{		
		// Apply rotation
		Turn(turn * DEGTORAD);
	}
	else
	{
		// Reset rotation
		Turn(0);
	}

	// WeelPrint
	if (GetKmh() > 50 && weelPrintStep <= 0)
	{
		// weelPrint 1
		mat4x4 tempMatrix;
		OrientWithCar(tempMatrix, { -1.2f, -0.4f,-3.2f });
		vec3 pos = vec3(tempMatrix.M[12], tempMatrix.M[13], tempMatrix.M[14]);

		// weelPrint 2
		mat4x4 tempMatrix2;
		OrientWithCar(tempMatrix2, { 1.2f, -0.4f,-3.2f });
		vec3 pos2 = vec3(tempMatrix2.M[12], tempMatrix2.M[13], tempMatrix2.M[14]);

		new WeelPrint(_app, 5, tempMatrix, Red);

		new WeelPrint(_app, 5, tempMatrix2, Red);

		weelPrintStep = 0.15;
	}

	// Limits
	UpdateRotateLimit();
	UpdateObserverDistance();
}

void Vehicle::PostUpdate()
{
	Render();

	// Shape Orientacion
	OrientWithCar(pipe_L.transform, { 0.9f , 0, -2.0f }, 90);
	pipe_L.Render();

	OrientWithCar(pipe_R.transform, { -0.9f , 0, -2.0f }, 90);
	pipe_R.Render();

	// Fake plane
	OrientWithCar(ground.transform, {0 , 1, 0 });
	ground.SetPos(ground.transform.M[12], -0.5f, ground.transform.M[14]);
	ground.Render();
}

void Vehicle::CleanUp()
{
}

void Vehicle::OnCollisionEnter(PhysBody3D* col)
{
	if (col->gameObject == nullptr) return;

	if (col->gameObject->tag == Tag::TaxiClient) printf("Client!\n");

	if (col->gameObject->tag == Tag::Booster)
	{
		boostCounter = boostCoolDown;
		boostOn = true;
		ApplyEngineForce(7000);
	}
}

void Vehicle::Respawn()
{
	pBody->SetLinearVelocity(0, 0, 0);

	pBody->body->setWorldTransform(defaultTransform);
}

vec3 Vehicle::GetObserverPos()
{
	vec3 ret;

	mat4x4 tempTransform;

	btVector3 tempOffset(observerPos.x * observerDistance, observerPos.y * observerDistance, observerPos.z * observerDistance);

	OrientWithCar(tempTransform, tempOffset);

	ret.Set(tempTransform.M[12], tempTransform.M[13], tempTransform.M[14]);

	return ret;
}

void Vehicle::Render()
{
	Cylinder wheel;

	wheel.color = Black;

	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info->wheels[0].radius;
		wheel.height = info->wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	Cube chassis(info->chassis_size.x, info->chassis_size.y, info->chassis_size.z);
	
	if (boostOn) chassis.color = Color(1, 0.5, 0);
	else chassis.color = Red;

	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info->chassis_offset.x, info->chassis_offset.y, info->chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();

	chassis.Render();
}

void Vehicle::ApplyEngineForce(float force)
{
	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if (info->wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

void Vehicle::Brake(float force)
{
	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if (info->wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

/// <summary>
///
/// </summary>
/// <param name="degrees"> THIS IS A RAD!!!!!!!</param>
void Vehicle::Turn(float degrees)
{
	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if (info->wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

float Vehicle::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}

void Vehicle::InitShapes()
{
	// Pipes
	pipe_L.color = Yellow;
	pipe_L.radius = 0.2f;
	pipe_L.height = 1.5f;

	pipe_R.color = Yellow;
	pipe_R.radius = 0.2f;
	pipe_R.height = 1.5f;

	// Fake Ground
	ground.color = Ground;
	ground.size = { 4000, 0.1, 4000};
}

void Vehicle::UpdateRotateLimit()
{
	int currentSpeed = GetKmh();

	if (abs(currentSpeed) < 50)
	{
		turn = 15.0f;
	}
	else if (abs(currentSpeed) < 75)
	{
		turn = 3.0f;
	}
	else if (abs(currentSpeed) < 250)
	{
		turn = 2.0f;
	}
	else
	{
		turn = 1.0f;
	}
}

void Vehicle::UpdateObserverDistance()
{
	int currentSpeed = GetKmh();

	float relative = (currentSpeed - 80) / 240.0f;

	float tempDistance = (observerMaxDistance - observerMinDistance) * relative;

	observerDistance = observerMaxDistance + tempDistance;	
}

/// <summary>
/// Orientation with car
/// </summary>
/// <param name="transform">transform that write the result</param>
/// <param name="offset">relative position from car&thisObejct</param>
/// <param name="angle">angle in axis Y, because the car just rot in this axis</param>
void Vehicle::OrientWithCar(mat4x4& transform, btVector3 offset, float angle)
{
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&transform);

	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();

	offset = offset.rotate(q.getAxis(), q.getAngle());

	transform.M[12] += offset.getX();
	transform.M[13] += offset.getY();
	transform.M[14] += offset.getZ();

	// If want rotation
	if(angle != 0)
	{
		float tempAngle = q.getAngle() * RADTODEG;

		if (tempAngle < 0.15f) transform.rotate(angle, { 0, 1, 0 });

		else transform.rotate((tempAngle)+angle, { q.getAxis().getX(), q.getAxis().getY(), q.getAxis().getZ() });
	}	
}
