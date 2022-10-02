#include "Headers.h"
#include "LayerEditor.h"
#include "Application.h"
#include "QuickSave.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

#include "ImWindowConfiguration.h"
#include "ImWindowAbout.h"
#include "ImWindowOpenGL.h"
#include "ModuleLayers.h"
#include "LayerGame.h"

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
	ImGuiIO& io = ImGui::GetIO(); (void)io;
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
	}

	const char* glsl_version = "#version 130";
	ImGui_ImplSDL2_InitForOpenGL(Application::Instance()->window->window, Application::Instance()->renderer3D->context);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Create ImGui editor windows.


    imWindows[(uint)ImWindowID::CONFIGURATION] = new ImWindowConfiguration();
    imWindows[(uint)ImWindowID::ABOUT] = new ImWindowAbout();
	imWindows[(uint)ImWindowID::OPENGL] = new ImWindowOpenGL();


	// Setup font
	io.Fonts->AddFontFromFileTTF("Assets/font.ttf", 17.0f, NULL);

	game = (LayerGame*)Application::Instance()->layers->layers[(uint)LayersID::GAME];

}

void LayerEditor::PreUpdate()
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
			if (ImGui::MenuItem("Configuration"))
			{
				imWindows[(uint)ImWindowID::CONFIGURATION]->isEnabled = true;
			}

			if (ImGui::MenuItem("About"))
			{
				imWindows[(uint)ImWindowID::ABOUT]->isEnabled = true;
			}

			if (ImGui::MenuItem("OpenGL Configuration"))
			{
				imWindows[(uint)ImWindowID::OPENGL]->isEnabled = true;
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	ImGui::Begin("Game");
	ImVec2 gameDimensions = ImGui::GetContentRegionAvail();

	if (gameDimensions.x != gameWidth || gameDimensions.y != gameHeight)
	{
		// If the size of this imgui window is different from the one stored.
		gameWidth = gameDimensions.x;
		gameHeight = gameDimensions.y;

		Application::Instance()->renderer3D->frameBuffer.SetDimensions(gameWidth, gameHeight);
	}

	ImGui::Image((ImTextureID)Application::Instance()->renderer3D->frameBuffer.GetTexture(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();

	ImGui::Begin("Scene testing");
	ImGui::DragFloat3("Sphere position", &game->sphereRender.GetMesh().position.x, 0.1f);
	ImGui::DragFloat3("Sphere scale", &game->sphereRender.GetMesh().scale.x, 0.1f);
	ImGui::Separator();
	ImGui::DragFloat3("Cube position", &game->meshRender.GetMesh().position.x, 0.1f);
	ImGui::DragFloat3("Cube scale", &game->meshRender.GetMesh().scale.x, 0.1f);
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
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}
}

void LayerEditor::Update()
{
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
