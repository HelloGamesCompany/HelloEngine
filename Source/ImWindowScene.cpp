#include "Headers.h"
#include "ImWindowScene.h"
#include "ModuleCamera3D.h"

ImWindowScene::ImWindowScene()
{
	windowName = "Scene";

	isEnabled = true;

	moduleCamera = Application::Instance()->camera;
}

ImWindowScene::~ImWindowScene()
{
}

void ImWindowScene::Update()
{
	ImGui::Begin(windowName.c_str());

	moduleCamera->updateSceneCamera = ImGui::IsWindowHovered();

	ImVec2 sceneDimensions = ImGui::GetContentRegionAvail();

	if (sceneDimensions.x != gameWidth || sceneDimensions.y != gameHeight)
	{
		// If the size of this imgui window is different from the one stored.
		gameWidth = sceneDimensions.x;
		gameHeight = sceneDimensions.y;
		Application::Instance()->camera->sceneCamera.ChangeAspectRatio((float)gameWidth / (float)gameHeight);
	}

	ImGui::Image((ImTextureID)moduleCamera->sceneCamera.frameBuffer.GetTexture(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
}