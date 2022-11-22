#include "Headers.h"
#include "ImWindowScene.h"
#include "ModuleCamera3D.h"
#include "ModuleResourceManager.h"
#include "ModuleLayers.h"
#include "LayerEditor.h"
#include "ModuleRenderer3D.h"

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

			DetectImGuizmoInput();

			ImGui::Image((ImTextureID)sceneCamera->frameBuffer.GetTexture(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));

			GameObject* selected = Application::Instance()->layers->editor->selectedGameObject;

			if (selected != nullptr)
			{
				float4x4 auxiliarMatrix = selected->transform->GetGlobalMatrix();
				auxiliarMatrix.Transpose();

				// Could be done only when one of the 4 variables changes.
				ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
				ImGuizmo::SetDrawlist();
				if (ImGuizmo::Manipulate(sceneCamera->GetViewMatrix(), sceneCamera->GetProjectionMatrix(), _imOperation, ImGuizmo::MODE::WORLD, &auxiliarMatrix.v[0][0]))
				{
					auxiliarMatrix.Transpose();
					selected->transform->SetLocalFromGlobal(auxiliarMatrix);
				}

			}

			if (!ImGuizmo::IsUsing())
				DetectClick();
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

void ImWindowScene::DetectClick()
{
	if (ImGui::IsWindowHovered() && Application::Instance()->input->GetMouseButton(1) == KEY_DOWN && Application::Instance()->input->GetKey(SDL_SCANCODE_LALT) != KEY_REPEAT)
	{
		// Get mouse position normalized inside this window-----------------------------------------------------
		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 windowSize = ImGui::GetWindowSize();

		ImVec2 normalizedPos = { (float)Application::Instance()->input->GetMouseX(), (float)Application::Instance()->input->GetMouseY() };

		normalizedPos = { normalizedPos.x - windowPos.x,  normalizedPos.y - windowPos.y };

		float halfWindowWidth = (float)windowSize.x / 2;
		float halfWindowHeight = (float)windowSize.y / 2;

		float positionX = normalizedPos.x - halfWindowWidth;
		float positionY = normalizedPos.y - halfWindowHeight;

		positionX /= halfWindowWidth;
		positionY /= halfWindowHeight;

		positionY *= -1.0f;

		//--------------------------------------------------------------------------------------------------------

		LineSegment line = sceneCamera->cameraFrustum.UnProjectLineSegment(positionX, positionY);
		GameObject* hitGameObject = Application::Instance()->renderer3D->RaycastFromMousePosition(line, sceneCamera);
		Application::Instance()->layers->editor->SetSelectGameObject(hitGameObject);

	}
}

void ImWindowScene::DetectImGuizmoInput()
{
	if (Application::Instance()->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		_imOperation = ImGuizmo::OPERATION::TRANSLATE;
	}
	else if (Application::Instance()->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		_imOperation = ImGuizmo::OPERATION::ROTATE;
	}
	else if (Application::Instance()->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		_imOperation = ImGuizmo::OPERATION::SCALE;
	}
}
