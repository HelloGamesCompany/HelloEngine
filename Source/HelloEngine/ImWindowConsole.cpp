#include "Headers.h"
#include "ImWindowConsole.h"
#include "Console.h"
#include "nommgr.h"
#include "assimp/cimport.h"
#include "mmgr.h"

using CSL = Console;

static struct aiLogStream stream;

void AssimpCallback(const char* message, char* user)
{
	std::string strM = message;
	CSL::S_Log("Assimp: " + strM);
}

ImWindowConsole::ImWindowConsole()
{
	windowName = "Console";

	isEnabled = true;

	stream.callback = AssimpCallback;
	aiAttachLogStream(&stream);
}

ImWindowConsole::~ImWindowConsole()
{

}

void ImWindowConsole::Update()
{
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
		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), true, ImGuiWindowFlags_HorizontalScrollbar);

		if(!_isCollapse) // if isn't collapsing
		{
			std::string* buffer = nullptr;

			uint cslSize = CSL::S_GetLog(&buffer);

			for (size_t i = 0; i < cslSize; buffer++, i++)
			{
				ImGui::Text((*buffer).c_str());
			}
		}
		else // if is collapsing
		{
			auto logs = CSL::S_GetCollapseLog();

			for (auto log: logs)
			{
				ImGui::Text((log.first + "\tcount: " + std::to_string(log.second)).c_str());
			}
		}

		// Automatically set scroll to bottom
		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())	ImGui::SetScrollHereY(1.0f);

		ImGui::EndChild();	

		ImGui::Text(" Hello Engine :) ");
	}
	ImGui::End();
}