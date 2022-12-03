#include "Headers.h"
#include "ImWindowProject.h"
#include "ModuleFiles.h"
#include "FileTree.hpp"
#include "ModuleResourceManager.h"

#include "ModuleWindow.h"
#include "ModuleLayers.h"
#include "TextureImporter.h"

ImWindowProject::ImWindowProject()
{
	windowName = "Project";
	isEnabled = true;

    //fileTree = ModuleFiles::S_GetFileTree("Assets");

    _app = Application::Instance();

    _window = _app->window->window;

    ModuleResourceManager::S_GetFileTree(_fileTree);

    // Init delete object;
    _deleteFile = nullptr;

    // Init rootNode & currentNode
    UpdateFileNodes();

    _app->input->AddOnDropListener(std::bind(&ImWindowProject::OnDrop, this, std::placeholders::_1));

    char* buffer = nullptr;
    int size = ModuleFiles::S_Load("Resources/Editor/Images/files.dds", &buffer);

    fileImageID = TextureImporter::LoadEditorDDS(buffer, size);

    RELEASE(buffer);

    buffer = nullptr;
    size = ModuleFiles::S_Load("Resources/Editor/Images/folder.dds", &buffer);

    folderImageID = TextureImporter::LoadEditorDDS(buffer, size);

    RELEASE(buffer);

    buffer = nullptr;
    size = ModuleFiles::S_Load("Resources/Editor/Images/model.dds", &buffer);

    modelImageID = TextureImporter::LoadEditorDDS(buffer, size);

    RELEASE(buffer)
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
        // Warning, is static!!!
        static float widthLeft = 200; // Init Size child 1
        static float widthRight = 1200; // Init Size child 2

        // Adjust window size
        ImVec2 windowSize = ImGui::GetWindowSize();

        ImGui::DrawSplitter(0, 10, &widthLeft, &widthRight, 100, 200);

        widthRight = (windowSize.x - widthLeft - 20);

        // Left window
        if (widthLeft > 0)
        {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.16f, 0.16f, 0.16f, 1));

            if (ImGui::BeginChild("ChildL", ImVec2(widthLeft, 0), true, ImGuiWindowFlags_HorizontalScrollbar))
            {
                DrawTreeNodePanelLeft(newDir, _rootNode, false);
            }
            ImGui::EndChild();

            ImGui::PopStyleColor();
        }

        ImGui::SameLine();

        // Right window
        if (widthRight > 0)
        {
            if (ImGui::BeginChild("ChildR", ImVec2(widthRight, 0), true, ImGuiWindowFlags_HorizontalScrollbar))
            {        
                DrawTreeNodePanelRight(newDir);
            }
            ImGui::EndChild();
        }
        
        // Change current directory
        if (newDir)
        {
            _fileTree->_currentDir = newDir;
        }
    }
    ImGui::End();

    // If have any file to delete, delete this
    if (_deleteFile)
    {
        ModuleFiles::S_Delete(_deleteFile->path);

        if (_deleteFile->metaPath != "none")
        {
            ModuleResourceManager::S_DeleteMetaFile(_deleteFile->metaPath);
        }

        _deleteFile = nullptr;

        UpdateFileNodes();
    }

    // JUST FOR TEST

    
    if (Application::Instance()->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
    {
        ModuleResourceManager::S_SerializeScene(Application::Instance()->layers->rootGameObject);
    }

    if (Application::Instance()->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
    {
        std::string path = _fileTree->_currentDir->path + Application::Instance()->layers->rootGameObject->name + ".HScene";

        Application::Instance()->layers->RequestLoadScene(path);
    }
    
}

void ImWindowProject::DrawTreeNodePanelLeft(Directory*& newDir, Directory* node, const bool drawFiles) const
{
    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_None | ImGuiTreeNodeFlags_OpenOnArrow;

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
    // Return 
    Directory* parent1 = nullptr;
    Directory* parent2 = nullptr;

    if (_fileTree->_currentDir->parent)
    {
        parent1 = _fileTree->_currentDir->parent;
    }
    if (parent1 && parent1->parent)
    {
        parent2 = parent1->parent;
    }
    if (parent2)
    {
        std::string pName = parent2->name + " > ##Return";
        if (ImGui::Button(pName.c_str()))
        {
            newDir = parent2;
        }
    }
    if (parent1)
    {
        std::string pName = parent1->name + " > ##Return";
        ImGui::SameLine();
        if (ImGui::Button(pName.c_str()))
        {
            newDir = parent1;
        }
    }

    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4, 0.4, 0.4, 255));
    if (ImGui::Button(_fileTree->_currentDir->name.c_str()))
    {
        // Do nothing here, just fixed space
    }
    ImGui::PopStyleColor(1);

    ImGui::Separator();

    int numOfColumns = (ImGui::GetContentRegionAvail().x / itemWidth) -1;
    if (numOfColumns == 0)
        numOfColumns++;

    ImGui::Columns(numOfColumns);

    // Folders
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5, 0.5, 1.0, 255));
    for (int i = 0; i < _fileTree->_currentDir->directories.size(); i++)
    {
        if (ImGui::ImageButton(std::to_string(i).c_str(), (ImTextureID)folderImageID, ImVec2(itemWidth, itemHeight)))
        {
            newDir = _fileTree->_currentDir->directories[i];
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(_fileTree->_currentDir->directories[i]->name.c_str());
        }
        ImGui::TextWrapped(_fileTree->_currentDir->directories[i]->name.c_str());
        ImGui::NextColumn();
    }
    ImGui::PopStyleColor(1);

    // Files
    for (int i = 0; i < _fileTree->_currentDir->files.size(); i++)
    {
        uint icon = 0;
        icon = _fileTree->_currentDir->files[i].metaFile.type == ResourceType::MODEL ? modelImageID : fileImageID;
        if (ImGui::ImageButton(std::to_string(i).c_str(), (ImTextureID)icon, ImVec2(itemWidth, itemHeight)))
        {
            _fileTree->_currentDir->files[i].pressed = !_fileTree->_currentDir->files[i].pressed;
        }

        if (ImGui::BeginPopupContextItem()) // <-- use last item id as popup id
        {
            if (ImGui::Button("Delete"))
            {
                _deleteFile = &_fileTree->_currentDir->files[i];
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(_fileTree->_currentDir->files[i].name.c_str());
        }

        // Drag file
        ResourceType type = ModuleFiles::S_GetResourceType(_fileTree->_currentDir->files[i].name);
        if (type == ResourceType::TEXTURE || type == ResourceType::MODEL)
        {
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
            {
                if (type == ResourceType::TEXTURE)
                {
                    // Find resource path
                    _dragUID = _fileTree->_currentDir->files[i].metaFile.UID;

                    // Set payload to carry the index of our item (could be anything)
                    ImGui::SetDragDropPayload("Texture", &_dragUID, sizeof(uint));
                }
                else
                {
                    _dragUID = _fileTree->_currentDir->files[i].metaFile.UID;

                    ImGui::SetDragDropPayload("Model", &_dragUID, sizeof(uint));
                }
                ImGui::EndDragDropSource();
            }
        }
        ImGui::TextWrapped(_fileTree->_currentDir->files[i].name.c_str());


        if (_fileTree->_currentDir->files[i].metaFile.type == ResourceType::MODEL && _fileTree->_currentDir->files[i].pressed)
        {
            ResourceModel* model = (ResourceModel*)ModuleResourceManager::resources[_fileTree->_currentDir->files[i].metaFile.UID];
            for (int j = 0; j < model->modelMeshes.size(); j++)
            {
                ImGui::Button(model->modelMeshes[j]->debugName.c_str(), ImVec2(0, itemHeight / 3));

                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
                {
                    // Find resource path
                    _dragUID = model->modelMeshes[j]->UID;

                    // Set payload to carry the index of our item (could be anything)
                    ImGui::SetDragDropPayload("Mesh", &_dragUID, sizeof(uint));
                 
                    ImGui::EndDragDropSource();
                }
            }
        }


        ImGui::NextColumn();
    }

    //if (ImGui::BeginPopupContextItem()) // <-- use last item id as popup id
    //{
    //    if (ImGui::Button("Delete"))
    //    {
    //        ImGui::CloseCurrentPopup();
    //    }
    //    ImGui::EndPopup();
    //}
}

void ImWindowProject::OnDrop(const std::string filePath)
{
    std::string pathNormalized = ModuleFiles::S_NormalizePath(filePath);

    // Use this for global path files
    ModuleFiles::S_ExternalCopy(pathNormalized, _fileTree->_currentDir->path);

    std::string relativePath = _fileTree->_currentDir->path + ModuleFiles::S_GetFileName(pathNormalized);

    // File case

    File file = File(relativePath, ModuleFiles::S_GetFileName(pathNormalized), _fileTree->_currentDir);//_fileTree->_currentDir->files.emplace_back(relativePath, ModuleFiles::S_GetFileName(pathNormalized), _fileTree->_currentDir);
    
    _fileTree->_currentDir->files.push_back(file);
}

void ImWindowProject::UpdateFileNodes()
{
    ModuleResourceManager::S_UpdateFileTree();

    if (ModuleResourceManager::S_GetFileTree(_fileTree))
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
        }
    }
    // When Global Windows has deselected -> just the instante
    else if (_isWindowFocus)
    {
        _isWindowFocus = false;
    }
}