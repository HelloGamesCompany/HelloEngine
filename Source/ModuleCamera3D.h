#ifndef __MODULECAMERA3D_H__
#define __MODULECAMERA3D_H__

#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "CameraObject.h"

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

	void RequestFrameBufferRegen(CameraObject* camera, int width, int height);

public:
	std::map<uint, CameraObject> gameCameras;
	CameraObject* activeGameCamera = nullptr;
	CameraObject sceneCamera;

	CameraObject* currentDrawingCamera = nullptr;

	bool updateSceneCamera;
	bool updateGameCamera;

private:
	CameraObject* frameBufferRegenCamera = nullptr;
	int newBufferWidth, newBufferHeight;
};

#endif // !__MODULECAMERA3D_H__