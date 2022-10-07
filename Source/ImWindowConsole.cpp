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

	if (ImGui::Begin(windowName.c_str(), &isEnabled, ImGuiWindowFlags_NoCollapse))
	{
		if (ImGui::SmallButton("Collaps"))
		{
			_isCollapse = !_isCollapse;
		}ImGui::SameLine();

		if (ImGui::SmallButton("Clear"))
		{
			CSL::S_ClearLog();
		}ImGui::SameLine();

		ImGui::Text(CSL::S_GetLogCounts());

		ImGui::Separator();

		// Reserve enough left-over height for 1 separator + 1 input text
		const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);

		if(!_isCollapse)
		{
			std::string* buffer = nullptr;

			uint cslSize = CSL::S_GetLog(&buffer);

			for (size_t i = 0; i < cslSize; buffer++, i++)
			{
				ImGui::Text((*buffer).c_str());
			}
		}
		else
		{
			auto map = CSL::S_GetCollapseLog();

			for (auto m:map)
			{
				ImGui::Text((m.first + "\tcount: " + std::to_string(m.second)).c_str());
			}
		}

		ImGui::EndChild();			
	}
	ImGui::End();

	num++;
}