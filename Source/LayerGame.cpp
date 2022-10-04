#include "Headers.h"
#include "LayerGame.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "MeshImporter.h"

LayerGame::LayerGame()
{
	
}

LayerGame::~LayerGame()
{
}

void LayerGame::Start()
{
	MeshImporter::LoadMesh("Assets/BakerHouse.fbx");
	//MeshImporter::LoadMesh("Assets/backpack/backpack.mtl");
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
	for (int i = 0; i < meshComponentTest.size(); i++)
	{
		RELEASE(meshComponentTest[i]);
	}
}
