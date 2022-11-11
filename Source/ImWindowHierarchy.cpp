#include "Headers.h"
#include "ImWindowHierarchy.h"
#include "GameObject.h"
#include "ModuleLayers.h"
#include "LayerEditor.h"
#include "ModuleRenderer3D.h"
#include "ModelRenderManager.h"
#include "ModuleResourceManager.h"

ImWindowHierarchy::ImWindowHierarchy()
{
	windowName = "Hierarchy";

    layerEditor = (LayerEditor*)Application::Instance()->layers->layers[LayersID::EDITOR];

    gameObjectsReference = &Application::Instance()->layers->gameObjects;

	isEnabled = true;

    base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap;
}

ImWindowHierarchy::~ImWindowHierarchy()
{
}

void ImWindowHierarchy::Update()
{
	if (ImGui::Begin(windowName.c_str(), &isEnabled))
	{
            DrawGameObjectChildren(gameObjectsReference->at(1));
        hasSelectedAGameObject = false;
        ImGui::BeginChild("DropArea");
        {

            if(popUpOpen || popUpGOpen) DrawOptions();
            // Detect window popUps
            if(ImGui::IsWindowHovered() && !hasSelectedAGameObject)
            {
                if (ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Right))
                {
                    popUpOpen = true;
                    layerEditor->SetSelectGameObject(nullptr);
                }
                else if (ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left))
                {
                    popUpOpen = false;
                }
            }
            else
            {
                if (ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left) ||
                    ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Right))
                {
                    popUpOpen = false;
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

                Application::Instance()->resource->LoadFile(drop);
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
    ImGuiTreeNodeFlags node_flags = base_flags;

    GameObject* temp = layerEditor->GetSelectedGameObject();

    if (gameObject == temp) node_flags |= ImGuiTreeNodeFlags_Selected;

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

    if (ImGui::BeginDragDropSource())
    {
        ImGui::SetDragDropPayload("GameObject", nullptr, 0);
        draggingGameObject = gameObject;
        ImGui::Text("Change game object parent");
        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget())
    {
        if (ImGui::AcceptDragDropPayload("GameObject"))
        {
            draggingGameObject->SetParent(gameObject);
            draggingGameObject = nullptr;
        }
        ImGui::EndDragDropTarget();
    }

    // Select gameObejct
    if ((ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left)))
    {
        if (ImGui::IsItemHovered() && gameObject->_parent != nullptr)
        {
            layerEditor->SetSelectGameObject(gameObject);
            hasSelectedAGameObject = true;
        }
    }
    else if (gameObject->_parent && ImGui::BeginPopupContextItem())
    {
        layerEditor->SetSelectGameObject(gameObject);
        hasSelectedAGameObject = true;

        //DrawOptions();
        popUpGOpen = true;
        ImGui::EndPopup();
    }
    else if (gameObject == temp)
    {
        hasSelectedAGameObject = false;
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
    if (isLeaf) ImGui::TreePop();
}

void ImWindowHierarchy::DrawOptions()
{
    int selectedShape = 0;
    std::string shapeNames[4] = { "Cube", "Sphere", "Cylinder", "Plane" };

    ImGui::OpenPopup("basicShapes");

    if (ImGui::BeginPopup("basicShapes"))
    {
        if (layerEditor->selectedGameObject != nullptr)
        {
            ImGui::TextColored(ImVec4(1, 1, 0, 1), "Delete GameObject"); ImGui::SameLine(-ImGui::GetWindowWidth());
            if (ImGui::Selectable("##"))
            {
                Application::Instance()->layers->editor->AddPopUpMessage("Cannot delete GameObjects in this version yet! Check console.");
                Console::S_Log("Cannot delete GameObjects yet. Because of the Undo/ Redo system, we need to implement this feature carefully. Therefore, it is not included in this version.");
                //layerEditor->selectedGameObject->Destroy();
            }
        }
        if (ImGui::Selectable("Create empty GameObject"))
        {
            GameObject* parent = layerEditor->selectedGameObject != nullptr ? layerEditor->selectedGameObject : Application::Instance()->layers->rootGameObject;
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
                Application::Instance()->renderer3D->modelRender.CreatePrimitive(layerEditor->selectedGameObject, (PrimitiveType)i);
            }
        }
        ImGui::EndPopup();
    }
}
