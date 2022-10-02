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

	float* GetProjectionMatrix() { return &ProjectionMatrix.M[0]; };

	//TODO: This should be a class that has 2 instances. Each for camera (game, editor)
	uint GetFrameBufferTexture();

public:
	ModelRenderManager modelRender;
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
	bool isVSync = false;

	uint FBO = 0; // Frame Buffer Object (contains the result of the entire frame texture)
	uint textureColorbuffer = 0;
	uint RBO = 0;

private:
	void SetFrameBuffer();

};

#endif // !__MODULERENDERER3D_H__