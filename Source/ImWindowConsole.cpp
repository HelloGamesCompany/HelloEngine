#include "Headers.h"
#include "ImWindowConsole.h"
#include "Console.h"

using CSL = Console;

ImWindowConsole::ImWindowConsole()
{
	windowName = "Console";

	isEnabled = true;

	CSL::S_Init();

	for (size_t i = 0; i < 10; i++)
	{
		CSL::S_Log("hello");
	}
}

ImWindowConsole::~ImWindowConsole()
{
	CSL::S_Close();
}

void ImWindowConsole::Update()
{
	static int num = 0;

	//CSL::S_Log("hello" + std::to_string(num));

	//ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);

	//ImGui::ShowDemoWindow();

	if (ImGui::Begin(windowName.c_str(), &isEnabled, ImGuiWindowFlags_NoCollapse))
	{
		if (ImGui::SmallButton("Save Log"))
		{
			CSL::S_SaveLog();
		}ImGui::SameLine();

		//if (ImGui::Selectable("Collapse", &_isCollapse, 0, ImVec2(50, 0))); ImGui::SameLine();

		if (ImGui::SmallButton("Collapse"))
		{
			_isCollapse = !_isCollapse;
		}ImGui::SameLine();

		if (ImGui::SmallButton("Clear"))
		{
			CSL::S_ClearLog();
		}ImGui::SameLine();

		ImGui::Text("Total Logs: %s", CSL::S_GetLogCounts());

		ImGui::Separator();

		// Reserve enough left-over height for 1 separator + 1 input text
		const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);

		if(!_isCollapse) // if is not collapse
		{
			std::string* buffer = nullptr;

			uint cslSize = CSL::S_GetLog(&buffer);

			for (size_t i = 0; i < cslSize; buffer++, i++)
			{
				ImGui::Text((*buffer).c_str());
			}
		}
		else // if is collapse
		{
			auto logs = CSL::S_GetCollapseLog();

			for (auto log: logs)
			{
				ImGui::Text((log.first + "\tcount: " + std::to_string(log.second)).c_str());
			}
		}

		ImGui::EndChild();			
	}
	ImGui::End();

	num++;
}