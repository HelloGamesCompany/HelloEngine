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
	if (ImGui::Begin(windowName.c_str(), &isEnabled, ImGuiWindowFlags_NoCollapse))
	{
		ImGui::CollapsingHeader(Console::GetLogCounts());

		ImGui::Text(Console::GetLog());
	}
	ImGui::End();
}