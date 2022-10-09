#include "Headers.h"
#include "LayerGame.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "MeshImporter.h"
#include "TextureImporter.h"

LayerGame::LayerGame()
{
	
}

LayerGame::~LayerGame()
{
}

void LayerGame::Start()
{
	/*uint newTexture = TextureImporter::ImportTexture("Assets/Baker_house.png");
	uint newTexture2 = TextureImporter::ImportTexture("Assets/Baker_house.png");*/

	/*MeshImporter::LoadMesh("Assets/BakerHouse.fbx");
	MeshImporter::LoadMesh("Assets/BakerHouse.fbx");*/
	//MeshImporter::LoadMesh("Assets/backpack/backpack.obj");
	/*MeshRenderComponent* cube = new MeshRenderComponent();
	cube->InitAsDefaultCube();

	meshComponentTest.push_back(cube);*/
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
