#include "Headers.h"
#include "ImWindowScene.h"
#include "ModuleCamera3D.h"

ImWindowScene::ImWindowScene()
{
	windowName = "Scene";

	isEnabled = true;

	moduleCamera = Application::Instance()->camera;
	sceneCamera = &Application::Instance()->camera->sceneCamera;
}

ImWindowScene::~ImWindowScene()
{
}

void ImWindowScene::Update()
{
	ImGui::Begin(windowName.c_str());

	moduleCamera->updateSceneCamera = ImGui::IsWindowHovered();

	ImVec2 sceneDimensions = ImGui::GetContentRegionAvail();

	if (sceneDimensions.x != sceneWidth || sceneDimensions.y != sceneHeight)
	{
		// If the size of this imgui window is different from the one stored.
		sceneWidth = sceneDimensions.x;
		sceneHeight = sceneDimensions.y;
		sceneCamera->ChangeAspectRatio((float)sceneWidth / (float)sceneHeight);
	}

	ImGui::Image((ImTextureID)sceneCamera->frameBuffer.GetTexture(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
}