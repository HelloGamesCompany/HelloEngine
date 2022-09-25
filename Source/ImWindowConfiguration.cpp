#include "Headers.h"
#include "ImWindowConfiguration.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleInput.h"

ImWindowConfiguration::ImWindowConfiguration() : ImWindow()
{
	windowName = "Configuration";
	isEnabled = true;

	for (int i = 0; i < 60; i++)
	{
		frames.emplace(120);
		frames2.emplace_back(120);
	}

	countCPU = SDL_GetCPUCount();

	systemRAM = SDL_GetSystemRAM();

	app = Application::Instance();

	windowWidth = &app->window->width;
	windowHeight = &app->window->height;

	isVSyncOn = SDL_GL_GetSwapInterval();
}

ImWindowConfiguration::~ImWindowConfiguration()
{
}

void ImWindowConfiguration::Update()
{
	//if (frames.size() >= 60) frames.pop();

	//frames.push(60);

	std::string framerate = "Framerate: " + std::to_string(ImGui::GetIO().Framerate);

	if (ImGui::Begin(windowName.c_str(), &isEnabled, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (ImGui::CollapsingHeader("Application", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::PlotHistogram("##Framerate", &frames2.front(), frames2.size(), 0, framerate.c_str(), 0.0f, 160.0f, ImVec2(300, 160));
			if (ImGui::SliderInt("FPS Limit", &frameLimit, 30, 120))
			{
				app->SetFPS(frameLimit);
			}
		}

		if (ImGui::CollapsingHeader("Window", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::TextWrapped("Window Size: ");
			ImGui::TextWrapped("Width:"); ImGui::SameLine();
			ImGui::TextColored(ImVec4(255, 255, 0, 255), std::to_string(*windowWidth).c_str());

			ImGui::TextWrapped("Height:"); ImGui::SameLine();
			ImGui::TextColored(ImVec4(255, 255, 0, 255), std::to_string(*windowHeight).c_str());

			//ImGui::NewLine();

			static float brightness = 1.0f;

			if (ImGui::SliderFloat("Brightness", &brightness, 0.2f, 1.0f))
			{
				app->window->SetBrightness(brightness);
			}

			//ImGui::NewLine();

			ImGui::Checkbox("VSync", &isVSyncOn);
			app->renderer3D->ToggleVSync(isVSyncOn);
			
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
		}
	}
	ImGui::End();
}