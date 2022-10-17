#include "Headers.h"
#include "ImWindowHierarchy.h"
#include "GameObject.h"
#include "ModuleLayers.h"
#include "LayerEditor.h"

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
        DrawGameObjectChildren(gameObjectsReference->at(1));
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

    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Left))
    {
        layerEditor->SetSelectGameObject(gameObject);
    }

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
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
