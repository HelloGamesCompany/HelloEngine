#include "Headers.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "SDL_opengl.h"
#include "XMLNode.h"
#include "ModuleXML.h"
#include "ModuleLayers.h"

ModuleRenderer3D::ModuleRenderer3D(bool start_enabled) : Module(start_enabled)
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	// Set Up OpenGL Attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	//Create context
	context = SDL_GL_CreateContext(app->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	GLenum er = glewInit();

	SDL_GL_MakeCurrent(app->window->window, context);

	if(ret == true)
	{
		//Use Vsync
		
		XMLNode renderNode = app->xml->GetConfigXML().FindChildBreadth("renderer");
		isVSync = renderNode.node.child("vsync").attribute("value").as_bool();
		ToggleVSync(isVSync);
		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			//LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			//LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			//LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		//lights[0].ref = GL_LIGHT0;
		//lights[0].ambient.Set(0.5f, 0.5f, 0.5f, 0.5f);
		//lights[0].diffuse.Set(1.0f, 1.0f, 1.0f, 0.5f);
		//lights[0].SetPos(0.0f, 0.0f, 2.5f);
		//lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
	/*	lights[0].Active(true);*/
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);

		// Enable opacity
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	modelRender.Init();

	// Projection matrix for
	OnResize(app->window->width, app->window->height);

	frameBuffer.SetBufferInfo();

	return ret;
}

// PreUpdate: clear buffer
UpdateStatus ModuleRenderer3D::PreUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(app->camera->GetViewMatrix());

	// light 0 on cam pos
	/*lights[0].SetPos(app->camera->Position.x, app->camera->Position.y, app->camera->Position.z);*/

	//for(uint i = 0; i < MAX_LIGHTS; ++i)
	//	lights[i].Render();

	frameBuffer.Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	return UpdateStatus::UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
UpdateStatus ModuleRenderer3D::PostUpdate()
{
	Application::Instance()->layers->DrawLayers();
	modelRender.Draw();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	Application::Instance()->layers->DrawEditor();

	SDL_GL_SwapWindow(app->window->window);

	return UpdateStatus::UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	XMLNode configNode = app->xml->GetConfigXML();

	configNode.node.child("renderer").child("vsync").attribute("value").set_value(isVSync);

	configNode.Save();

	SDL_GL_DeleteContext(context);

	return true;
}

void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 2048.0f);

	app->window->width = width;
	app->window->height = height;
}

void ModuleRenderer3D::ToggleVSync(bool isOn)
{
	isVSync = isOn;
	SDL_GL_SetSwapInterval(isVSync);
}

void ModuleRenderer3D::ToggleOpenGLSystem(bool enable, int openGLMacro)
{
	if (enable) glEnable(openGLMacro);
	else glDisable(openGLMacro);
}

void ModuleRenderer3D::ToggleOpenGLWireframe(bool enable)
{
	if (enable)glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


