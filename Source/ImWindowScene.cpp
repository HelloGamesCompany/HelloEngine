#include "Headers.h"
#include "ImWindowScene.h"
#include "ModuleCamera3D.h"
#include "ModuleResourceManager.h"

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
	if (ImGui::Begin(windowName.c_str()))
	{
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Mesh"))
			{
				//Drop asset from Asset window to scene window
				const std::string drop = *(std::string*)payload->Data;

				Application::Instance()->resource->LoadFile(drop);
			}
			ImGui::EndDragDropTarget();
		}

		sceneCamera->active = true;
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
	}
	else
	{
		sceneCamera->active = false;
	}
	ImGui::End();
}