#include "Headers.h"
#include "ImWindowProject.h"
#include "ModuleFiles.h"
#include "FileTree.hpp"
#include "ModuleResourceManager.h"

#include "ModuleWindow.h"

ImWindowProject::ImWindowProject()
{
	windowName = "Project";
	isEnabled = true;

    //fileTree = ModuleFiles::S_GetFileTree("Assets");

    _app = Application::Instance();

    _window = _app->window->window;

    _resource = _app->resource;

    _resource->GetFileTree(_fileTree);

    // Init rootNode & currentNode
    UpdateFileNodes();

    _app->input->AddOnDropListener(std::bind(&ImWindowProject::OnDrop, this, std::placeholders::_1));
}

ImWindowProject::~ImWindowProject()
{
    _app->input->ClearOnDropListener();
}

void ImWindowProject::Update()
{
    CheckWindowFocus();

   	if (ImGui::Begin(windowName.c_str(), &isEnabled, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar))
	{
        // Options, Filter
        //if (ImGui::Button("Options")) ImGui::OpenPopup("Options");
        //ImGui::SameLine();
        //filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
        //ImGui::SameLine();

        // For change directory
        Directory* newDir = nullptr;

        // Resize children width
        static float width1 = 200; // Init Size child 1
        static float width2 = 1200; // Init Size child 2

        if (width2 < 200)
        {
            width2 = 200;
        }

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
                DrawTreeNodePanelLeft(newDir, _rootNode, false);
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
                DrawTreeNodePanelRight(newDir);
            }
            ImGui::EndChild();
        }
        
        if (newDir)
        {
            _fileTree->_currentDir = newDir;
        }
    }
    ImGui::End();
}

void ImWindowProject::DrawTreeNodePanelLeft(Directory*& newDir, Directory* node, const bool drawFiles) const
{
    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_None;

    if(node == _fileTree->_currentDir)
    {
        node_flags |= ImGuiTreeNodeFlags_Selected;
    }
    if(node->directories.empty())
    {
        node_flags |= ImGuiTreeNodeFlags_Leaf;
    }

    if (ImGui::TreeNodeEx(node->name.c_str(), node_flags))
    {
        // Slect node
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
        {
            newDir = node;
        }

        // Recursive functions
        for (int i = 0; i < node->directories.size(); i++)
        {
            DrawTreeNodePanelLeft(newDir, node->directories[i], drawFiles);
        }
        if(drawFiles)
        {
            for (int i = 0; i < node->files.size(); i++)
            {
                ImGui::Text(node->files[i].name.c_str());
            }
        }
        ImGui::TreePop();
    }
}

void ImWindowProject::DrawTreeNodePanelRight(Directory*& newDir)
{
    if (ImGui::Button("Return"))
    {
        if (_fileTree->_currentDir->parent)
        {
            newDir = _fileTree->_currentDir->parent;
        }
    }
    ImGui::Separator();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5, 0.5, 1.0, 255));
    for (int i = 0; i < _fileTree->_currentDir->directories.size(); i++)
    {
        if (ImGui::Button(_fileTree->_currentDir->directories[i]->name.c_str(), ImVec2(110, 50)))
        {
            newDir = _fileTree->_currentDir->directories[i];
        }
        ImGui::SameLine();
    }
    ImGui::PopStyleColor(1);
    for (int i = 0; i < _fileTree->_currentDir->files.size(); i++)
    {
        ImGui::Button(_fileTree->_currentDir->files[i].name.c_str(), ImVec2(110, 50));

        ResourceType type = ModuleFiles::S_GetResourceType(_fileTree->_currentDir->files[i].name);

        if (type == ResourceType::TEXTURE || type == ResourceType::MESH)
        {
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
            {
                if (type == ResourceType::TEXTURE)
                {
                    _dragPath = ModuleFiles::S_GetFileName(_fileTree->_currentDir->files[i].name, false);

                    _dragPath = "Resources/Textures/" + _dragPath + ".dds";

                    // Set payload to carry the index of our item (could be anything)
                    ImGui::SetDragDropPayload("Texture", &_dragPath, sizeof(std::string));
                }
                else
                {
                    _dragPath = _fileTree->_currentDir->path + _fileTree->_currentDir->files[i].name;

                    ImGui::SetDragDropPayload("Mesh", &_dragPath, sizeof(std::string));
                }
                ImGui::EndDragDropSource();
            }
        }
        ImGui::SameLine();
    }
}

void ImWindowProject::OnDrop(const std::string filePath)
{
    std::string pathNormalized = ModuleFiles::S_NormalizePath(filePath);

    // Use this for global path files
    ModuleFiles::S_ExternalCopy(pathNormalized, _fileTree->_currentDir->path);

    // File case
    File file = _fileTree->_currentDir->files.emplace_back(pathNormalized, ModuleFiles::S_GetFileName(pathNormalized), _fileTree->_currentDir);

    //std::string file = currentNode->path + ModuleFiles::S_GetFileName(pathNormalized, true);

    _resource->ImportFile(file.path);

    // Folder case

    //RELEASE(fileTree);

    //fileTree = ModuleFiles::S_GetFileTree("Assets");

    //currentNode = fileTree;
}

void ImWindowProject::UpdateFileNodes()
{
    _resource->UpdateFileTree();

    if (_resource->GetFileTree(_fileTree))
    {
        _fileTree->GetRootDir(_rootNode);
    }
}

void ImWindowProject::CheckWindowFocus()
{
    Uint32 flags = SDL_GetWindowFlags(_window);

    // When Global Windows has selected -> just the instante
    if (((flags & SDL_WINDOW_INPUT_FOCUS) != 0))
    {
        if (!_isWindowFocus)
        {
            _isWindowFocus = true;

            UpdateFileNodes();

            //std::cout << "FOCUS" << std::endl;
        }
    }
    // When Global Windows has deselected -> just the instante
    else if (_isWindowFocus)
    {
        _isWindowFocus = false;
        //std::cout << "NO FOCUS" << std::endl;
    }
}