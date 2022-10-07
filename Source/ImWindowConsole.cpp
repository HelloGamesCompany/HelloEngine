#include "Headers.h"
#include "ImWindowConsole.h"
#include "Console.h"

ImWindowConsole::ImWindowConsole()
{
	windowName = "Console";

	isEnabled = true;
}

ImWindowConsole::~ImWindowConsole()
{
}

void ImWindowConsole::Update()
{
	Console::S_Log("hello");

	static int num = 0;

	//ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);

	if (ImGui::Begin(windowName.c_str(), &isEnabled, ImGuiWindowFlags_NoCollapse))
	{
		if (ImGui::SmallButton("Collaps"))
		{
			// TODO: Collaps text
		}ImGui::SameLine();

		if (ImGui::SmallButton("Clear"))
		{
			Console::S_ClearLog();
		}ImGui::SameLine();

		ImGui::Text(Console::S_GetLogCounts());

		ImGui::Separator();

		// Reserve enough left-over height for 1 separator + 1 input text
		const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);

		ImGui::Text(Console::S_GetLog());

		ImGui::EndChild();
	}
	ImGui::End();

	num++;
}