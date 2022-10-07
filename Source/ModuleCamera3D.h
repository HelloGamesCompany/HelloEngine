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

	void Look(const float3&Position, const float3&Reference, bool RotateAroundReference = false);
	void LookAt(const float3&Spot);
	void Move(const float3&Movement);
	float* GetViewMatrix();

	static void Tets() { std::cout << "hello" << std::endl; }

private:

	void CalculateViewMatrix();

public:
	
	float3 X, Y, Z, Position, Reference;

private:

	float4x4 ViewMatrix, ViewMatrixInverse;
	float3 offset;
};

#endif // !__MODULECAMERA3D_H__