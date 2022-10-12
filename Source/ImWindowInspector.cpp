#include "Headers.h"
#include "ImWindowInspector.h"

ImWindowInspector::ImWindowInspector()
{
	windowName = "Inspector";

	isEnabled = true;
}

ImWindowInspector::~ImWindowInspector()
{
}

void ImWindowInspector::Update()
{
	if (ImGui::Begin(windowName.c_str(), &isEnabled))
	{
		if (selectGameobject)
		{
				
		}
	}
	ImGui::End();
}
