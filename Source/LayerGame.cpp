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
	meshRender.InitAsDefaultCube();
	sphereRender.InitAsDefaultSphere();

	for (int i = 0; i < 100; i++)
	{
		MeshRenderComponent newMesh;
		newMesh.InitAsDefaultCube({ 1.0f + i, 0.0f,0.0f });
	}
	for (int i = 0; i < 100; i++)
	{
		MeshRenderComponent newMesh;
		newMesh.InitAsDefaultSphere({ 0.0f, 1.0f+i,0.0f });
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

}

void LayerGame::CleanUp()
{

}
