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

    base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
}

ImWindowHierarchy::~ImWindowHierarchy()
{
}

void ImWindowHierarchy::Update()
{
    ImGui::ShowDemoWindow();

	if (ImGui::Begin(windowName.c_str(), &isEnabled))
	{
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

        DrawGameObjectChildren(gameObjectsReference->at(1));
	    
        if ((ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Right) && ImGui::IsWindowHovered()) || popUpOpen)
        {
            popUpOpen = true;
            int selectedShape = 0;
            std::string shapeNames[4] = { "Cube", "Sphere", "Cylinder", "Plane"};

            ImGui::OpenPopup("basicShapes");
            
            if (ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Left) && !ImGui::IsAnyItemHovered())
            {
                std::cout << "Close popup" << std::endl;
                popUpOpen = false;
            }
            if(ImGui::BeginPopup("basicShapes"))
            {
                ImGui::Text("Select Shape");
                ImGui::Separator();
                for (int i = 0; i < 4; i++)
                    if (ImGui::Selectable(shapeNames[i].c_str()))
                    {
                        selectedShape = i;
                        Application::Instance()->renderer3D->modelRender.CreatePrimitive(rightClickedGameObject, (PrimitiveType)i);
                        popUpOpen = false;
                        rightClickedGameObject = nullptr;
                    }
                ImGui::EndPopup();
            }
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

    if (gameObject == layerEditor->selectedGameObject) node_flags |= ImGuiTreeNodeFlags_Selected;

    bool node_open;

    if (gameObject->_children.empty())
    {
        node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        ImGui::TreeNodeEx((void*)(intptr_t)iteration, node_flags, gameObject->name.c_str(), iteration);
        node_open = false;
    }
    else
    {
        node_open = ImGui::TreeNodeEx((void*)(intptr_t)iteration, node_flags, gameObject->name.c_str(), iteration);
    }

    if (ImGui::BeginDragDropSource())
    {
        ImGui::SetDragDropPayload("GameObject", gameObject, sizeof(GameObject*));

        draggingGameObject = gameObject;

        ImGui::Text("Change game object parent");
        ImGui::EndDragDropSource();
    }

    if (ImGui::IsItemHovered())
    {
        if (ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Left)) layerEditor->SetSelectGameObject(gameObject);
        if (ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Right)) rightClickedGameObject = gameObject;
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

    if (node_open)
    {
        if (!gameObject->_children.empty()) DrawGameObjectChildren(gameObject, true); 
        ImGui::TreePop();
    }
}
