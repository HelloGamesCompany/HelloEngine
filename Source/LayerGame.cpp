#include "Headers.h"
#include "LayerGame.h"
#include "Application.h"
#include "ModuleWindow.h"

LayerGame::LayerGame()
{
	
}

LayerGame::~LayerGame()
{
}

void LayerGame::Start()
{
	VertexBufferArraySetUp();
	Mesh cube;
	cube.InitAsCube({ 5.0f,1.0f,5.0f }, { 1.0f,1.0f,1.0f });
	renderManager1.SetMeshInformation(cube);

	Mesh sphere;
	sphere.InitAsSphere({ 1.0f,1.0f,1.0f }, { 1.0f,1.0f,1.0f });
	renderManager2.SetMeshInformation(sphere);

	for (int i = 0; i < 10; i++)
	{
		Mesh newMesh;
		newMesh.InitAsMeshInformation({ 1.0f + i, 1.0f, 1.0f }, { 2.0f,1.0f,1.0f });
		renderManager1.AddMesh(newMesh);
		renderManager2.AddMesh(newMesh);
	}

}

void LayerGame::PreUpdate()
{
}

void LayerGame::Update()
{
}

void LayerGame::PostUpdate()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	renderManager1.Draw();
	renderManager2.Draw();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Draw cube using Pirmitive (ModernOpenGL)
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//cube->InnerRender();
}

void LayerGame::CleanUp()
{

}

void LayerGame::VertexBufferArraySetUp()
{
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// generate texture
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Application::Instance()->window->width, Application::Instance()->window->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	// create render buffer object
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Application::Instance()->window->width, Application::Instance()->window->height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
}
