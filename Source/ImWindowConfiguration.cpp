#include "Headers.h"
#include "ImWindowConfiguration.h"
#include "Application.h"

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
}

ImWindowConfiguration::~ImWindowConfiguration()
{
}

void ImWindowConfiguration::Update()
{
	//if (frames.size() >= 60) frames.pop();

	//frames.push(60);

	std::string framerate = "Framerate: " + std::to_string(ImGui::GetIO().Framerate);

	if (configOpen)
	{
		if (ImGui::Begin(windowName.c_str(), &configOpen, ImGuiWindowFlags_NoCollapse))
		{
			if (ImGui::CollapsingHeader("Application", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::PlotHistogram("##Framerate", &frames2.front(), frames2.size(), 0, framerate.c_str(), 0.0f, 160.0f, ImVec2(300, 160));
					//ImGui::PlotHistogram()
					ImGui::SliderInt("##FPS Limit", &frameLimit, 30, 120);
					//ImGui::SliderInt("FPS AAAA", &a, 10, 360);	
			}

			if (ImGui::CollapsingHeader("Window", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Checkbox("Fullscreen", &fullscreen);
			}

			if (ImGui::CollapsingHeader("Hardware", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Text(std::to_string(ImGui::GetIO().Framerate).c_str());
				
				ImGui::Text("CPU Count: "); ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
				ImGui::Text(std::to_string(countCPU).c_str());
				ImGui::PopStyleColor();

				ImGui::Text("RAM: "); ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
				ImGui::Text(std::to_string(systemRAM).c_str());
				ImGui::PopStyleColor();
				
				ImGui::BulletText("asdasdas");
			}
		}
		ImGui::End();
	}
}