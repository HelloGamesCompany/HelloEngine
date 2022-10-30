#include "Headers.h"
#include "ImWindowGame.h"
#include "ModuleCamera3D.h"
#include "CameraObject.h"

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
	if(ImGui::Begin(windowName.c_str()))
	{
		/*currentGameCamera->active = true;*/
		ImVec2 gameDimensions = ImGui::GetContentRegionAvail();

		if (gameDimensions.x != gameWidth || gameDimensions.y != gameHeight)
		{
			// If the size of this imgui window is different from the one stored.
			gameWidth = gameDimensions.x;
			gameHeight = gameDimensions.y;
			currentGameCamera->ChangeAspectRatio((float)gameWidth / (float)gameHeight);
			//moduleCameras->RequestFrameBufferRegen(currentGameCamera, gameWidth, gameHeight);
		}

		if (currentGameCamera->active)ImGui::Image((ImTextureID)currentGameCamera->frameBuffer.GetTexture(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));
	}
	else
	{
		/*currentGameCamera->active = true;*/
	}
	ImGui::End();
}
