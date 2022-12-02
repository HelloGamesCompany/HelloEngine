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
	Time::SetTimeScale(2.0f);
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
	if ((!_isPlaying || _paused) && !_oneFrame)
	{
		_update = false;
		return;
	}
	_oneFrame = false;
	_update = true;
	
	// Update time.
	Time::UpdateRealTime();
	Time::UpdateGameTime();

	//std::cout << "Real time: " << Time::RealTimeDeltaTime() << std::endl;
	//std::cout << "Game time: " << Time::GameDeltaTime() << std::endl;s
	/*std::cout << "Total Time Real Time: " << Time::GameTimeInRealTimeCount() << std::endl;
	std::cout << "Total Time Game Time: " << Time::GameTimeCount() << std::endl;*/
	std::cout << "Frame count: " << Time::GameFrameCount() << std::endl;
}

void LayerGame::Update()
{
	if (!_update)
		return;
}

void LayerGame::PostUpdate()
{
	if (!_update)
		return;
}

void LayerGame::Play()
{
	// TODO: Save scene.

	_isPlaying = true;
	_paused = false;
}

void LayerGame::Stop()
{
	Time::Reset();
	_isPlaying = false;
	// TODO: Reload scene.
}

void LayerGame::Pause()
{
	_isPlaying = false;
	_paused = true;
}

void LayerGame::OneFrame()
{
	_oneFrame = true;
}

void LayerGame::CleanUp()
{
}
