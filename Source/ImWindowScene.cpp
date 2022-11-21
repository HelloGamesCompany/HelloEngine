#include "Headers.h"
#include "ImWindowScene.h"
#include "ModuleCamera3D.h"
#include "ModuleResourceManager.h"
#include "ModuleLayers.h"
#include "LayerEditor.h"

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
		ImGui::BeginChild("DropArea");
		{
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
			//ImGuizmo::DrawGrid(Application::Instance()->camera->sceneCamera.GetViewMatrix(), Application::Instance()->camera->sceneCamera.GetProjectionMatrix(), &identity.v[0][0], 100);

			ImGui::Image((ImTextureID)sceneCamera->frameBuffer.GetTexture(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));

			GameObject* selected = Application::Instance()->layers->editor->selectedGameObject;

			if (selected != nullptr)
			{
				float4x4 auxiliarMatrix = selected->transform->GetGlobalMatrix();
				auxiliarMatrix.Transpose();

				ImGuizmo::SetOrthographic(false);

				ImGuiIO& io = ImGui::GetIO();
				ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

				ImGuizmo::SetDrawlist();
				if (ImGuizmo::Manipulate(sceneCamera->GetViewMatrix(), sceneCamera->GetProjectionMatrix(), ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::MODE::WORLD, &auxiliarMatrix.v[0][0]))
				{
					selected->transform->SetPosition(auxiliarMatrix.Transposed().TranslatePart());
				}

			}
		}
		ImGui::EndChild();

		// Create Droped mesh
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Mesh"))
			{
				const std::string drop = *(std::string*)payload->Data;

				Application::Instance()->resource->LoadFile(drop);

				std::string popUpmessage = "Loaded Mesh: " + drop;
				Application::Instance()->layers->editor->AddPopUpMessage(popUpmessage);

			}
			ImGui::EndDragDropTarget();
		}
	}
	else
	{
		sceneCamera->active = false;
	}
	
	ImGui::End();
}