#include "Headers.h"
#include "ImWindowConfiguration.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleFiles.h"
#include "CycleArray.hpp"

ImWindowConfiguration::ImWindowConfiguration() : ImWindow()
{
	windowName = "Configuration";

	isEnabled = true;

	frames = new CArrayF(60, 0.0f);

	app = Application::Instance();

	countCPU = SDL_GetCPUCount();

	systemRAM = SDL_GetSystemRAM();

	windowWidth = &ModuleWindow::width;

	windowHeight = &ModuleWindow::height;

	windowBrightness = &ModuleWindow::brightness;

	isVSyncOn = &app->renderer3D->isVSync;

	frameLimit = app->frameCap;

	automaticCompilation = ModuleFiles::S_IsMSBuildOn();

	// Init render configurations
	moduleRenderer = Application::Instance()->renderer3D;

	// Get openGl node with module xml
	XMLNode openGlNode = Application::Instance()->xml->GetConfigXML().FindChildBreadth("openGL");

	// Init setting values with node context
	for (pugi::xml_node n = openGlNode.node.first_child(); !n.empty(); n = n.next_sibling())
	{
		n.name();
		renderConfigs.insert(
			std::make_pair(
				n.name(), // map key -> conifg name
				std::make_pair(	// map value -> pair<value,tag>
					n.attribute("value").as_bool(false),	// config value
					n.attribute("tag").as_int(0))));	// config tag
	}
}

ImWindowConfiguration::~ImWindowConfiguration()
{
	RELEASE(frames);

	// Save values to the xml file
	XMLNode openGlNode = Application::Instance()->xml->GetConfigXML().FindChildBreadth("openGL");

	for (pugi::xml_node n = openGlNode.node.first_child(); !n.empty(); n = n.next_sibling())
		n.attribute("value").set_value(renderConfigs[n.name()].first);

	openGlNode.Save();
}

void ImWindowConfiguration::Update()
{
	std::string framerate = "Framerate: " + std::to_string(ImGui::GetIO().Framerate);

	frames->push_back(ImGui::GetIO().Framerate);

	if (ImGui::Begin(windowName.c_str(), &isEnabled, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (ImGui::CollapsingHeader("Application", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::PlotHistogram("##Framerate", frames->front(), frames->size(), 0, framerate.c_str(), 0.0f, 160.0f, ImVec2(300, 160));
			if (ImGui::SliderInt("FPS Limit", &frameLimit, 30, 120))
			{
				app->SetFPS(frameLimit);
			}
			ImGui::Separator();
			if (ModuleFiles::S_IsMSBuildOn())
			{
				if (ImGui::Checkbox("Automatic Compilation", &automaticCompilation))
					ModuleFiles::S_SetAutomaticCompilation(automaticCompilation);
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
				ModuleWindow::S_SetBrightness(*windowBrightness);
			}

			ImGui::Checkbox("VSync", isVSyncOn);
			app->renderer3D->ToggleVSync(*isVSyncOn);	
		}

		if (ImGui::CollapsingHeader("Game Time", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::TextWrapped("\Time variables\t");

			ImGui::TextWrapped("Real time delta time: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(255, 255, 0, 255), "%.4f", EngineTime::RealTimeDeltaTime()); 
			ImGui::TextWrapped("Real time total time: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(255, 255, 0, 255), "%.4f", EngineTime::GameTimeInRealTimeCount());


			ImGui::TextWrapped("Game delta time: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(255, 255, 0, 255), "%.4f", EngineTime::GameDeltaTime());
			ImGui::TextWrapped("Game total time: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(255, 255, 0, 255), "%.4f", EngineTime::GameTimeCount());
			ImGui::TextWrapped("Game frame count: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d", EngineTime::GameFrameCount());

			float tempTimeScale = EngineTime::GetTimeScale();
			if (ImGui::DragFloat("Time scale", &tempTimeScale, 0.05f))
				EngineTime::SetTimeScale(tempTimeScale);
		}

		if (ImGui::CollapsingHeader("Input", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::TextWrapped("\tMouse Input\t");

			ImGui::TextWrapped("Mouse Position: x = "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d", ModuleInput::S_GetMouseX()); ImGui::SameLine();
			ImGui::TextWrapped(" y = "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d", ModuleInput::S_GetMouseY());
		}

		if(ImGui::CollapsingHeader("Render", ImGuiTreeNodeFlags_DefaultOpen))
		{
			// iter.first = config name,
			// iter.second.first = conifg value,
			// iter.second.second = config openGL tag
			for (auto& iter : renderConfigs)
			{
				ImGui::Checkbox(iter.first.c_str(), &iter.second.first);
				if (iter.first == "wireframe") moduleRenderer->ToggleOpenGLWireframe(iter.second.first);
				else moduleRenderer->ToggleOpenGLSystem(iter.second.first, iter.second.second);
			}
		}

		if (ImGui::CollapsingHeader("Hardware", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::TextWrapped("CPU Count: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(255, 255, 0, 255), std::to_string(countCPU).c_str());

			ImGui::TextWrapped("RAM: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(255, 255, 0, 255), std::to_string(systemRAM).c_str());

			ImGui::Separator();
			ImGui::TextWrapped("Vendor %s", glGetString(GL_VENDOR));
			ImGui::TextWrapped("Renderer: %s", glGetString(GL_RENDERER));
			ImGui::TextWrapped("OpenGL version supported %s", glGetString(GL_VERSION));
			ImGui::TextWrapped("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

		}
	}
	ImGui::End();
}