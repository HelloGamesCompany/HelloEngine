#include "Headers.h"
#include "ImWindowProject.h"
#include "ModuleFiles.h"
#include "FileTree.hpp"
#include "ModuleResourceManager.h"

ImWindowProject::ImWindowProject()
{
	windowName = "Project";
	isEnabled = true;

    fileTree = ModuleFiles::S_GetFileTree("Assets");

    currentNode = fileTree;

    Application::Instance()->input->AddOnDropListener(std::bind(&ImWindowProject::OnDrop, this, std::placeholders::_1));
}

ImWindowProject::~ImWindowProject()
{
    Application::Instance()->input->ClearOnDropListener();

    RELEASE(fileTree);
}

void ImWindowProject::Update()
{
   	if (ImGui::Begin(windowName.c_str(), &isEnabled, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar))
	{
        // Resize children widths
        static float width1 = 200; // Init Size child 1
        static float width2 = 1200; // Init Size child 2
        static float windowInitX = ImGui::GetWindowSize().x;

        // For change directory
        FileTree* newDir = nullptr;

        if (width2 < 200)
        {
            width2 = 200;
        }

        // Options, Filter
        //if (ImGui::Button("Options")) ImGui::OpenPopup("Options");
        //ImGui::SameLine();
        //filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
        //ImGui::SameLine();
        if (ImGui::Button("Return")) 
        {
            if (!currentNode->isRoot())
            {
                newDir = currentNode->GetParent();
            }
        }
        ImGui::Separator();

        // Adjust window size
        ImVec2 windowSize = ImGui::GetWindowSize();

        ImGui::DrawSplitter(0, 10, &width1, &width2, 100, 200);       

        width2 = (windowSize.x - width1 - 20);

        // Left window
        if (width1 > 0) 
        {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.16f, 0.16f, 0.16f, 1));

            if (ImGui::BeginChild("ChildL", ImVec2(width1, 0), true, ImGuiWindowFlags_HorizontalScrollbar))
            {
                DrawTreeNode(fileTree, false);            
            }
            ImGui::EndChild();
            ImGui::PopStyleColor();
        }
   
        // Right window
        if (width2 > 0)
        {
            ImGui::SameLine();

            if (ImGui::BeginChild("ChildR", ImVec2(width2, 0), true, ImGuiWindowFlags_HorizontalScrollbar))
            {        
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5, 0.5, 1.0, 255));
                for (int i = 0; i < currentNode->directories.size(); i++)
                {             
                    if(ImGui::Button(currentNode->directories[i]->name.c_str(), ImVec2(110, 50)))
                    {
                        newDir = currentNode->directories[i];
                    }
                    ImGui::SameLine();
                }
                ImGui::PopStyleColor(1);
                for (int i = 0; i < currentNode->files.size(); i++)
                {
                    ImGui::Button(currentNode->files[i].c_str(), ImVec2(110, 50));

                    ResourceType type = ModuleFiles::S_GetResourceType(currentNode->files[i]);

                    if (type == ResourceType::TEXTURE || type == ResourceType::MESH)
                    {
                        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
                        {
                            if(type == ResourceType::TEXTURE)
                            {
                                dragPath = ModuleFiles::S_GetFileName(currentNode->files[i], false);

                                dragPath = "Resources/Textures/" + dragPath + ".dds";

                                // Set payload to carry the index of our item (could be anything)
                                ImGui::SetDragDropPayload("Texture", &dragPath, sizeof(std::string));
                            }
                            else
                            {
                                dragPath = currentNode->path + currentNode->files[i];

                                ImGui::SetDragDropPayload("Mesh", &dragPath, sizeof(std::string));
                            }
                            ImGui::EndDragDropSource();
                        }
                    }
                    ImGui::SameLine();
                }              
            }
            ImGui::EndChild();
        }
        if (newDir)
        {
            currentNode = newDir;
        }
    }
    ImGui::End();
}

void ImWindowProject::DrawTreeNode(const FileTree* node, bool drawFiles) const
{
    if (ImGui::TreeNode(node->name.c_str()))
    {
        for (int i = 0; i < node->directories.size(); i++)
        {
            DrawTreeNode(node->directories[i], drawFiles);
        }
        if(drawFiles)
        {
            for (int i = 0; i < node->files.size(); i++)
            {
                ImGui::Text(node->files[i].c_str());
            }
        }
        ImGui::TreePop();
    }
}

void ImWindowProject::OnDrop(std::string filePath)
{
    std::string pathNormalized = ModuleFiles::S_NormalizePath(filePath);

    // Use this for global path files
    ModuleFiles::S_ExternalCopy(pathNormalized, currentNode->path);

    currentNode->files.push_back(ModuleFiles::S_GetFileName(pathNormalized));

    std::string file = currentNode->path + ModuleFiles::S_GetFileName(pathNormalized, true);

    Application::Instance()->resource->ImportFile(file);

    //RELEASE(fileTree);

    //fileTree = ModuleFiles::S_GetFileTree("Assets");

    //currentNode = fileTree;
}