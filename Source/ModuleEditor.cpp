#include "Headers.h"
#include "ModuleEditor.h"
#include "Application.h"
#include "QuickSave.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

#include "ImWindowConfiguration.h"
#include "ImWindowAbout.h"

ModuleEditor::ModuleEditor(bool enabled) :Module(enabled)
{
}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Start()
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

    return true;
}

UpdateStatus ModuleEditor::PreUpdate()
{
    return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModuleEditor::Update()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	static bool showWindow = false;

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Application"))
		{
			if (ImGui::MenuItem("Close Appplication"))
			{
				return UpdateStatus::UPDATE_STOP;
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

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}



	static bool asset = true;

	if (asset)
	{
		ImGui::Begin("Assets", &asset);
		ImGui::End();
	}

	ImGui::Begin("Viewport");
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

    return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModuleEditor::PostUpdate()
{
    return UpdateStatus::UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
	for (int i = 0; i < (uint)ImWindowID::MAX; i++)
	{
		RELEASE(imWindows[i]);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

    return true;
}
