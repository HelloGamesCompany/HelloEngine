#include "Headers.h"
#include "ImWindowGame.h"
#include "ModuleCamera3D.h"
#include "CameraObject.h"
#include "LayerGame.h"

ImWindowGame::ImWindowGame()
{
	windowName = "Game";
}

ImWindowGame::~ImWindowGame()
{
}

void ImWindowGame::Update()
{
	if(ImGui::Begin(windowName.c_str()))
	{
		LayerGame::detectInput = ImGui::IsWindowFocused();
		if (Application::Instance()->camera->activeGameCamera != nullptr) 
		{
			ImVec2 gameDimensions = ImGui::GetContentRegionAvail();

			if (gameDimensions.x != gameWidth || gameDimensions.y != gameHeight)
			{
				// If the size of this imgui window is different from the one stored.
				gameWidth = gameDimensions.x;
				gameHeight = gameDimensions.y;
				Application::Instance()->camera->activeGameCamera->ChangeAspectRatio((float)gameWidth / (float)gameHeight);
			}

			if (Application::Instance()->camera->activeGameCamera->active)
			{
				ImGui::Image((ImTextureID)Application::Instance()->camera->activeGameCamera->frameBuffer.GetTexture(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));
			}
		}
	}

	ImGui::End();
}
