#include "Headers.h"
#include "ImWindowHierarchy.h"

ImWindowHierarchy::ImWindowHierarchy()
{
	windowName = "Hierarchy";

	isEnabled = true;
}

ImWindowHierarchy::~ImWindowHierarchy()
{
}

void ImWindowHierarchy::Update()
{
	if (ImGui::Begin(windowName.c_str(), &isEnabled))
	{

	}
	ImGui::End();
}
