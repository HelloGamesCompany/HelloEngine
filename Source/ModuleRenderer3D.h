#ifndef __MODULERENDERER3D_H__
#define __MODULERENDERER3D_H__

#include "Module.h"
#include "ModelRenderManager.h"

class CameraObject;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	UpdateStatus PreUpdate();
	UpdateStatus PostUpdate();
	bool CleanUp();

	void OnResize(int width, int height);
	void ToggleVSync(bool isOn);

	void ToggleOpenGLSystem(bool enable, int openGLMacro);

	void ToggleOpenGLWireframe(bool enable);

	GameObject* RaycastFromMousePosition(LineSegment& ray, CameraObject* camera);


public:
	ModelRenderManager modelRender;
	SDL_GLContext context;
	bool isVSync = false;

private:
	ModuleCamera3D* cameras = nullptr;

};

#endif // !__MODULERENDERER3D_H__