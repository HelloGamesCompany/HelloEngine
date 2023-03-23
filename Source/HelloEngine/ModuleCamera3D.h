#ifndef __MODULECAMERA3D_H__
#define __MODULECAMERA3D_H__

#include "Module.h"
#include "Globals.h"
#include "CameraObject.h"
#include "SceneCameraObject.h"

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

	void DrawCameraFrustums();

	void RequestFrameBufferRegen(CameraObject* camera, int width, int height);

	CameraObject* CreateGameCamera();

	void SetCurrentActiveGameCamera(CameraObject* target);

	void EraseGameCamera(CameraObject* erasedCamera);

public:
	std::vector<CameraObject*> gameCameras;

	CameraObject* activeGameCamera = nullptr;

	SceneCameraObject* sceneCamera = nullptr;
	SceneCameraObject* UICamera = nullptr;

	CameraObject* currentDrawingCamera = nullptr;

	bool updateSceneCamera = true;

	bool updateGameCamera = true;
private:
	std::vector<std::pair<CameraObject*, std::pair<int, int>>>  _frameBufferRegenCamera;
};

#endif // !__MODULECAMERA3D_H__