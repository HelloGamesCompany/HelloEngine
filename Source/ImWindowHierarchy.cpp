#include "Headers.h"
#include "ImWindowHierarchy.h"
#include "GameObject.h"
#include "ModuleLayers.h"
#include "LayerEditor.h"
#include "ModuleRenderer3D.h"
#include "ModelRenderManager.h"
#include "ModuleResourceManager.h"
#include "ModuleCommand.h"

ImWindowHierarchy::ImWindowHierarchy()
{
	windowName = "Hierarchy";

    isEnabled = true;

    _app = Application::Instance();

    _layerEditor = (LayerEditor*)_app->layers->layers[LayersID::EDITOR];

    _gameObjectsReference = &_app->layers->gameObjects;

    _popUpOpen = false;

    _hasSelectedAGameObject = false;

    _draggingGameObject = nullptr;

    _base_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap;
}

ImWindowHierarchy::~ImWindowHierarchy()
{
}

void ImWindowHierarchy::Update()
{
	if (ImGui::Begin(windowName.c_str(), &isEnabled))
    {
        _hasSelectedAGameObject = false;
        
        DrawGameObjectChildren(_gameObjectsReference->at(1));

        ImGui::BeginChild("DropArea");
        {
            // Draw PopUps
            if (_popUpOpen)
            {            
                if (ImGui::BeginPopup("basicShapes"))
                {
                    DrawOptions();

                    ImGui::EndPopup();
                }

                ImGui::OpenPopup("basicShapes", ImGuiPopupFlags_MouseButtonMask_);
            }

            // Detect window PopUps
            if(ImGui::IsWindowHovered() && !_hasSelectedAGameObject)
            {
                if (ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Right))
                {
                    _popUpOpen = true;
                    _layerEditor->SetSelectGameObject(nullptr);
                }
            }
            else
            {
                if (ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left) ||
                    ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Right))
                {
                    _popUpOpen = false;
                }
            }      
        }
        ImGui::EndChild();

        // Create Droped mesh
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Mesh"))
            {
                //Drop asset from Asset window to scene window
                const std::string drop = *(std::string*)payload->Data;

                _app->resource->LoadFile(drop);
            }
            ImGui::EndDragDropTarget();
        }
    }
	ImGui::End();
}

void ImWindowHierarchy::DrawGameObjectChildren(GameObject* gameObject, bool onlyChildren)
{
    if (!onlyChildren) ProcessGameObject(gameObject, 0);
    else
    {
        for (int i = 0; i < gameObject->_children.size(); i++)
        {
            ProcessGameObject(gameObject->_children[i], i);
        }
    }
}

void ImWindowHierarchy::ProcessGameObject(GameObject* gameObject, int iteration)
{
    if (gameObject->_isPendingToDelete) 
        return;

    ImGuiTreeNodeFlags node_flags = _base_flags;

    GameObject* temp = _layerEditor->GetSelectedGameObject();

    if (gameObject == temp)
    {
        node_flags |= ImGuiTreeNodeFlags_Selected;
    }

    bool node_open;
    bool isLeaf = false;
    ImGui::AlignTextToFramePadding();

    if (gameObject->_children.empty())
    {
        isLeaf = true;
        node_flags |= ImGuiTreeNodeFlags_Leaf;
        ImGui::TreeNodeEx((void*)(intptr_t)iteration, node_flags, gameObject->name.c_str(), iteration);
        node_open = false;
    }
    else
    {
        node_open = ImGui::TreeNodeEx((void*)(intptr_t)iteration, node_flags, gameObject->name.c_str(), iteration);
    }

    // Drag & drop
    if (ImGui::BeginDragDropSource())
    {
        ImGui::SetDragDropPayload("GameObject", nullptr, 0);
        _draggingGameObject = gameObject;
        ImGui::Text("Change game object parent");
        ImGui::EndDragDropSource();
    }
    if (ImGui::BeginDragDropTarget())
    {
        if (ImGui::AcceptDragDropPayload("GameObject"))
        {
            _draggingGameObject->SetParent(gameObject);
            _draggingGameObject = nullptr;
        }
        ImGui::EndDragDropTarget();
    }

    // Select gameObejct
    if (ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left) || 
        ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Right))
    {
        if (ImGui::IsItemHovered() && gameObject->_parent != nullptr)
        {
            _layerEditor->SetSelectGameObject(gameObject);
            _hasSelectedAGameObject = true;
        }
    }

    // Draw PopUps
    if(gameObject == _layerEditor->GetSelectedGameObject())
    {
        if (gameObject->_parent)
        {
            if (ImGui::BeginPopupContextItem("basicShapes"))
            {
                DrawOptions();

                ImGui::EndPopup();
            }
        }
        else
        {
            _hasSelectedAGameObject = false;
        }
    }
 
    ImGui::SameLine(ImGui::GetWindowWidth() - 20);
    if (ImGui::SmallButton(gameObject->IsActive() ? "X" : " "))
    {
        gameObject->SetActive(!gameObject->IsActive());
    }

    if (node_open)
    {
        if (!gameObject->_children.empty()) DrawGameObjectChildren(gameObject, true); 
        ImGui::TreePop();
    }
    if (isLeaf)
    {
        ImGui::TreePop();
    }
}

void ImWindowHierarchy::DrawOptions()
{
    int selectedShape = 0;
    std::string shapeNames[4] = { "Cube", "Sphere", "Cylinder", "Plane" };

    if (_layerEditor->selectedGameObject != nullptr)
    {
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Delete GameObject"); ImGui::SameLine(-ImGui::GetWindowWidth());
        if (ImGui::Selectable("##"))
        {
#ifdef STANDALONE           
             _app->command->S_DeleteGameObject(_layerEditor->selectedGameObject);
#else
            _layerEditor->selectedGameObject->Destroy()
#endif // STANDALONE
        }
    }
    if (ImGui::Selectable("Create empty GameObject"))
    {
        GameObject* parent = _layerEditor->selectedGameObject != nullptr ? _layerEditor->selectedGameObject : Application::Instance()->layers->rootGameObject;
        GameObject* newGameObject = new GameObject(parent, "Empty");
    }

    ImGui::Separator();
    ImGui::Text("Select Shape");
    ImGui::Separator();

    for (int i = 0; i < 4; i++)
    {
        if (ImGui::Selectable(shapeNames[i].c_str()))
        {
            selectedShape = i;
            _app->renderer3D->modelRender.CreatePrimitive(_layerEditor->selectedGameObject, (PrimitiveType)i);
        }
    }
}