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
        if (ImGui::TreeNode("Root"))
        {
            for (int i = 0; i < 8; i++)
            {
                if (i == 0)  ImGui::SetNextItemOpen(true, ImGuiCond_Once);

                if (ImGui::TreeNode((void*)(intptr_t)i, "GameObject %d", i))
                {
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }
	}
	ImGui::End();
}