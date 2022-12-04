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
	Time::Reset();
	Time::Start();
	_isPlaying = true;
	/*_paused = false;*/
}

void LayerGame::Stop()
{
	Time::Reset();
	_isPlaying = false;
	_paused = false;
	// TODO: Reload scene.
}

void LayerGame::Pause()
{
	//if (!_isPlaying)
	//	return;

	_paused = !_paused;
	if (!_paused)
		Time::Start();
}

void LayerGame::OneFrame()
{
	if (!_isPlaying || !_paused) return;
	Time::Start();
	_oneFrame = true;
}

void LayerGame::CleanUp()
{
}
