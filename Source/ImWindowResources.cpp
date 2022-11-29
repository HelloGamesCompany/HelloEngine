#include "Headers.h"
#include "ImWindowResources.h"
#include "ModuleResourceManager.h"

ImWindowResources::ImWindowResources()
{
	windowName = "Resource Testing";
}

ImWindowResources::~ImWindowResources()
{
}

void ImWindowResources::Update()
{
	if (ImGui::Begin(windowName.c_str()))
	{
		for (auto& resource : ModuleResourceManager::resources)
		{
			ImGui::Text(resource.second->debugName.c_str()); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 0, 1, 1), std::to_string(resource.second->referenceCount).c_str());
		}
	}
	ImGui::End();
}
