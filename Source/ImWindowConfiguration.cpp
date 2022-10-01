#include "Headers.h"
#include "ImWindowConfiguration.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleInput.h"


#include "ModulePhysFS.h"

ImWindowConfiguration::ImWindowConfiguration() : ImWindow()
{
	windowName = "Configuration";

	isEnabled = true;

	frames = new CArrayF(60, 0.0f);

	app = Application::Instance();

	countCPU = SDL_GetCPUCount();

	systemRAM = SDL_GetSystemRAM();

	windowWidth = &app->window->width;

	windowHeight = &app->window->height;

	windowBrightness = &app->window->brightness;

	isVSyncOn = &app->renderer3D->isVSync;

	frameLimit = &app->frameCap;

	ModulePhysFS* mP = new ModulePhysFS();

	mP->ReadTest();

	RELEASE(mP);
}

ImWindowConfiguration::~ImWindowConfiguration()
{
	RELEASE(frames);
}

// ESTO LLAMA DESPUES DE SU DESTRUCTOR??????
void ImWindowConfiguration::Update()
{
	std::string framerate = "Framerate: " + std::to_string(ImGui::GetIO().Framerate);

	frames->push_back(ImGui::GetIO().Framerate);

	if (ImGui::Begin(windowName.c_str(), &isEnabled, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (ImGui::CollapsingHeader("Application", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::PlotHistogram("##Framerate", frames->front(), frames->size(), 0, framerate.c_str(), 0.0f, 160.0f, ImVec2(300, 160));
			if (ImGui::SliderInt("FPS Limit", frameLimit, 30, 120))
			{
				app->SetFPS(*frameLimit);
			}
		}

		if (ImGui::CollapsingHeader("Window", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::TextWrapped("Window Size: ");
			ImGui::TextWrapped("Width:"); ImGui::SameLine();
			ImGui::TextColored(ImVec4(255, 255, 0, 255), std::to_string(*windowWidth).c_str());

			ImGui::TextWrapped("Height:"); ImGui::SameLine();
			ImGui::TextColored(ImVec4(255, 255, 0, 255), std::to_string(*windowHeight).c_str()); ImGui::SameLine();

			ImGui::HelpMarker("Shows Window Width and Height");

			if (ImGui::SliderFloat("Brightness", windowBrightness, 0.2f, 1.0f))
			{
				app->window->SetBrightness(*windowBrightness);
			}

			ImGui::Checkbox("VSync", isVSyncOn);
			app->renderer3D->ToggleVSync(*isVSyncOn);
			
		}

		if (ImGui::CollapsingHeader("Input", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::TextWrapped("\tMouse Input\t");

			ImGui::TextWrapped("Mouse Position: x = "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d", app->input->GetMouseX()); ImGui::SameLine();
			ImGui::TextWrapped(" y = "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d", app->input->GetMouseY()); 

		}

		if (ImGui::CollapsingHeader("Hardware", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::TextWrapped(std::to_string(ImGui::GetIO().Framerate).c_str());

			ImGui::TextWrapped("CPU Count: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(255, 255, 0, 255), std::to_string(countCPU).c_str());

			ImGui::TextWrapped("RAM: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(255, 255, 0, 255), std::to_string(systemRAM).c_str());

			ImGui::TextWrapped("--------OpenGL-------- ");
			ImGui::TextWrapped("Vendor %s", glGetString(GL_VENDOR));
			ImGui::TextWrapped("Renderer: %s", glGetString(GL_RENDERER));
			ImGui::TextWrapped("OpenGL version supported %s", glGetString(GL_VERSION));
			ImGui::TextWrapped("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

		}
	}
	ImGui::End();
}