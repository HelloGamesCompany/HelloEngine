#ifndef __MODULERENDERER3D_H__
#define __MODULERENDERER3D_H__

#include "Module.h"
#include "Light.h"

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

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
	bool isVSync = false;
};

#endif // !__MODULERENDERER3D_H__