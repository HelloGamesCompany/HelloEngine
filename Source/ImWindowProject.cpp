#include "Headers.h"
#include "ImWindowProject.h"

ImWindowProject::ImWindowProject()
{
	windowName = "Project";
	isEnabled = true;
}

ImWindowProject::~ImWindowProject()
{
}

void ImWindowProject::Update()
{
    ImGui::SetNextWindowSizeConstraints(ImVec2(400, 200), ImVec2(600, 400));

	if (ImGui::Begin(windowName.c_str(), &isEnabled, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar))
	{
        // Options, Filter
        if (ImGui::Button("Options")) ImGui::OpenPopup("Options");
        ImGui::SameLine();
        filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
        ImGui::Separator();

        ImVec2 windowSize = ImGui::GetWindowSize();

        // Resize children widths
        static float width1 = 200; // Init Size child 1
        static float width2 = 1200; // Init Size child 2
        static float windowInitX = windowSize.x;

        Console::S_Log("Windth: " + std::to_string(width2));

        ImGui::DrawSplitter(0, 10, &width1, &width2, 50, 200);       

        width2 = (windowSize.x - width1 - 20);

        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.16f, 0.16f, 0.16f, 1));

        if(ImGui::BeginChild("ChildL", ImVec2(width1, 0), true, ImGuiWindowFlags_HorizontalScrollbar))
        {
            if (ImGui::TreeNode("Assets"))
            {
                for (int i = 0; i < 8; i++)
                {
                    // Use SetNextItemOpen() so set the default state of a node to be open. We could
                    // also use TreeNodeEx() with the ImGuiTreeNodeFlags_DefaultOpen flag to achieve the same thing!
                    if (i == 0)
                        ImGui::SetNextItemOpen(true, ImGuiCond_Once);

                    if (ImGui::TreeNode((void*)(intptr_t)i, "Child %d", i))
                    {
                        ImGui::Text("blah blah");
                        ImGui::SameLine();
                        if (ImGui::SmallButton("button")) {}
                        ImGui::TreePop();
                    }
                }
                ImGui::TreePop();
            }
            ImGui::EndChild();
        }

        ImGui::PopStyleColor();

        ImGui::SameLine();

        if(ImGui::BeginChild("ChildR", ImVec2(width2, 0), true, ImGuiWindowFlags_HorizontalScrollbar))
        {
            if (ImGui::TreeNode("Basic trees"))
            {
                for (int i = 0; i < 8; i++)
                {
                    // Use SetNextItemOpen() so set the default state of a node to be open. We could
                    // also use TreeNodeEx() with the ImGuiTreeNodeFlags_DefaultOpen flag to achieve the same thing!
                    if (i == 0)
                        ImGui::SetNextItemOpen(true, ImGuiCond_Once);

                    if (ImGui::TreeNode((void*)(intptr_t)i, "Child %d", i))
                    {
                        ImGui::Text("blah blah");
                        ImGui::SameLine();
                        if (ImGui::SmallButton("button")) {}
                        ImGui::TreePop();
                    }
                }
                ImGui::TreePop();
            }
            ImGui::EndChild();
        }
      
	}
	ImGui::End(); 
}
