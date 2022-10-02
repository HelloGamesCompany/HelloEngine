#ifndef __MODULECAMERA3D_H__
#define __MODULECAMERA3D_H__

#include "Module.h"
#include "Globals.h"
#include "glmath.h"

class Vehicle;

//TODO: There should be multiple camera instances.
//TODO: Each camera instance should have a framebuffer, which texture size should change with the window the camera is displayed in.
// At the same time, the camera Aspect ratio should change with said window dimensions.

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	UpdateStatus Update();
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();

	void SetTarget(Vehicle* tg, vec3 offset);
	void RemoveTarget();

	static void Tets() { std::cout << "hello" << std::endl; }

private:

	void CalculateViewMatrix();

public:
	
	vec3 X, Y, Z, Position, Reference;

private:

	mat4x4 ViewMatrix, ViewMatrixInverse;

	Vehicle* target = nullptr;
	vec3 offset;
};

#endif // !__MODULECAMERA3D_H__