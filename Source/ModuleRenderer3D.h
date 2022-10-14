#ifndef __MODULERENDERER3D_H__
#define __MODULERENDERER3D_H__

#include "Module.h"
#include "Light.h"
#include "ModelRenderManager.h"

#define MAX_LIGHTS 8

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

	float* GetProjectionMatrix() { return &ProjectionMatrix; };

public:
	ModelRenderManager modelRender;
	SDL_GLContext context;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
	bool isVSync = false;

private:
	ModuleCamera3D* cameras = nullptr;

};

#endif // !__MODULERENDERER3D_H__