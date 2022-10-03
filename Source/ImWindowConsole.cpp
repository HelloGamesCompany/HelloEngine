#include "Headers.h"
#include "ImWindowConsole.h"

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
		ImGui::Text("hello Console");
	}
	ImGui::End();
}