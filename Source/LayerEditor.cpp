#include "Headers.h"
#include "LayerEditor.h"
#include "Application.h"
#include "QuickSave.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"

#include "ImWindowConfiguration.h"
#include "ImWindowAbout.h"
#include "ImWindowOpenGL.h"
#include "ImWindowConsole.h"
#include "ImWindowProject.h"
#include "ImWindowHierarchy.h"
#include "ImWindowInspector.h"
#include "ImWindowQuickSave.h"

#include "ModuleLayers.h"
#include "LayerGame.h"

#include "ModuleFiles.h"
#include "TransformComponent.h"

LayerEditor::LayerEditor()
{
}

LayerEditor::~LayerEditor()
{
}

void LayerEditor::Start()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	//ImGui::StyleColorsClassic();
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 10.0f;
		style.Colors[ImGuiCol_WindowBg].w = 0.0f;
		style.Colors[ImGuiCol_Border] = ImVec4(255, 1, 1, 255);
	}

	// Setup ImGui style
	style.Colors[ImGuiCol_ScrollbarBg] = style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1f, 0.1f, 0.1f, 1);

	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, 1);

	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.2f, 0.2f, 0.2f, 1);

	style.Colors[ImGuiCol_FrameBg] = style.Colors[ImGuiCol_Button] = style.Colors[ImGuiCol_TitleBg] = style.Colors[ImGuiCol_Header] = 
		style.Colors[ImGuiCol_TitleBgActive]= style.Colors[ImGuiCol_TabUnfocused]= style.Colors[ImGuiCol_Tab]=
		ImVec4(0.25f, 0.25f, 0.25f, 1);

	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1);

	style.Colors[ImGuiCol_ButtonHovered] = style.Colors[ImGuiCol_HeaderHovered] = style.Colors[ImGuiCol_FrameBgActive]=
		style.Colors[ImGuiCol_DockingPreview] = style.Colors[ImGuiCol_TabActive] = style.Colors[ImGuiCol_SliderGrab] =
		ImVec4(0.4f, 0.4f, 0.4f, 1);

	style.Colors[ImGuiCol_ButtonActive] = style.Colors[ImGuiCol_TitleBgCollapsed] = style.Colors[ImGuiCol_SliderGrabActive] =
		style.Colors[ImGuiCol_HeaderActive] = style.Colors[ImGuiCol_TabHovered] = style.Colors[ImGuiCol_CheckMark] =
		ImVec4(0.65f, 0.65f, 0.65f, 1);

	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.95f, 0.95f, 0.95f, 1);

	// Setup font
	char* buf = nullptr;

	uint bufSize = ModuleFiles::S_Load("Resources/Editor/Fonts/font.ttf", &buf);

	// ImGui will delete buf memory when close application, so no need realease here
	io.Fonts->AddFontFromMemoryTTF(buf, bufSize, 14.0f, NULL);

	// Init OpenGL
	const char* glsl_version = "#version 130";
	ImGui_ImplSDL2_InitForOpenGL(Application::Instance()->window->window, Application::Instance()->renderer3D->context);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Create ImGui editor windows.
    imWindows[(uint)ImWindowID::CONFIGURATION] = new ImWindowConfiguration();
    imWindows[(uint)ImWindowID::ABOUT] = new ImWindowAbout();
	imWindows[(uint)ImWindowID::OPENGL] = new ImWindowOpenGL();
	imWindows[(uint)ImWindowID::CONSOLE] = new ImWindowConsole();
	imWindows[(uint)ImWindowID::PROJECT] = new ImWindowProject();
	imWindows[(uint)ImWindowID::QUICKSAVE] = new ImWindowQuickSave();
	imWindows[(uint)ImWindowID::INSPECTOR] = new ImWindowInspector();
	imWindows[(uint)ImWindowID::HIERARCHY] = new ImWindowHierarchy();

	game = (LayerGame*)Application::Instance()->layers->layers[(uint)LayersID::GAME];
}

void LayerEditor::PreUpdate()
{

}

void LayerEditor::Update()
{
}


void LayerEditor::PostUpdate()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	if (ImGui::BeginMainMenuBar())
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.05f, 0.05f, 0.05f, 1));

		ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.95f, 0.95f, 0.95f, 1));

		if (ImGui::BeginMenu("Application"))
		{
			if (ImGui::MenuItem("Close Appplication"))
			{
				// TODO: Exit application
			}

			ImGui::EndMenu();	
		}

		if (ImGui::BeginMenu("Windows"))
		{
			for (int i = 0; i < (uint)ImWindowID::MAX; i++)
			{
				if (ImGui::MenuItem(imWindows[i]->windowName.c_str()))
				{
					imWindows[i]->isEnabled = true;
				}
			}
			ImGui::EndMenu();
		}

		ImGui::PopStyleColor(2);

		ImGui::EndMainMenuBar();
	}

	ImGui::Begin("Scene");
	
	Application::Instance()->camera->updateSceneCamera = ImGui::IsWindowHovered();

	ImVec2 sceneDimensions = ImGui::GetContentRegionAvail();

	if (sceneDimensions.x != gameWidth || sceneDimensions.y != gameHeight)
	{
		// If the size of this imgui window is different from the one stored.
		gameWidth = sceneDimensions.x;
		gameHeight = sceneDimensions.y;

		Application::Instance()->camera->RequestFrameBufferRegen(&Application::Instance()->camera->sceneCamera, gameWidth, gameHeight);
	}

	ImGui::Image((ImTextureID)Application::Instance()->camera->sceneCamera.frameBuffer.GetTexture(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();

	ImGui::Begin("Transform testing");
	
	LayerEditor* editorLayer = (LayerEditor*)Application::Instance()->layers->layers[LayersID::EDITOR];
	GameObject* selectedGO = editorLayer->selectedGameObject;

	if (selectedGO != nullptr)
	{
		float3 tempPos = selectedGO->GetComponent<TransformComponent>()->localTransform.position;
		float3 tempRot = selectedGO->GetComponent<TransformComponent>()->localTransform.rotation;
		float3 tempScale = selectedGO->GetComponent<TransformComponent>()->localTransform.scale;

		if (ImGui::DragFloat3("Transform position", &tempPos[0], 0.1f))
		{
			selectedGO->GetComponent<TransformComponent>()->SetPosition(tempPos);
		}

		if (ImGui::DragFloat3("Transform rotation", &tempRot[0], 0.1f))
		{
			selectedGO->GetComponent<TransformComponent>()->SetRotation(tempRot);
		}

		if (ImGui::DragFloat3("Transform scale", &tempScale[0], 0.1f))
		{
			selectedGO->GetComponent<TransformComponent>()->SetScale(tempScale);
		}
	}
	ImGui::End();

    for (int i = 0; i < (uint)ImWindowID::MAX; i++)
    {
        if (imWindows[i]->isEnabled)
            imWindows[i]->Update();
    }

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
	//  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}
}

void LayerEditor::CleanUp()
{
	for (int i = 0; i < (uint)ImWindowID::MAX; i++)
	{
		RELEASE(imWindows[i]);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}