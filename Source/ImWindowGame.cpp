#include "Headers.h"
#include "ImWindowGame.h"
#include "ModuleCamera3D.h"

ImWindowGame::ImWindowGame()
{
	windowName = "Game";
	moduleCameras = Application::Instance()->camera;
	currentGameCamera = moduleCameras->activeGameCamera;
}

ImWindowGame::~ImWindowGame()
{
}

void ImWindowGame::Update()
{
	ImGui::Begin(windowName.c_str());

	ImVec2 sceneDimensions = ImGui::GetContentRegionAvail();

	if (sceneDimensions.x != gameWidth || sceneDimensions.y != gameHeight)
	{
		// If the size of this imgui window is different from the one stored.
		gameWidth = sceneDimensions.x;
		gameHeight = sceneDimensions.y;
		moduleCameras->RequestFrameBufferRegen(currentGameCamera, gameWidth, gameHeight);
	}

	ImGui::Image((ImTextureID)currentGameCamera->frameBuffer.GetTexture(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
}
