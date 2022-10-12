#include "Headers.h"
#include "ImWindowQuickSave.h"

ImWindowQuickSave::ImWindowQuickSave()
{
	windowName = "Quick Save";

	isEnabled = true;
}

ImWindowQuickSave::~ImWindowQuickSave()
{
}

void ImWindowQuickSave::Update()
{
	if (ImGui::Begin(windowName.c_str(), &isEnabled))
	{
	
	}
	ImGui::End();
}