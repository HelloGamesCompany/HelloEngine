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
        if (ImGui::TreeNode("Root"))
        {
            DrawGameObjectChildren(gameObjectsReference->at(1), 1);
            ImGui::TreePop();
        }
	}

	ImGui::End();
}

void ImWindowHierarchy::DrawGameObjectChildren(GameObject* gameObject, int layer)
{
    for (int i = 0; i < gameObject->_children.size(); i++)
    {
        if (i == 0)  ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        
        ImGuiTreeNodeFlags node_flags = base_flags;

        if (gameObject->_children[i] == layerEditor->selectedGameObject) node_flags |= ImGuiTreeNodeFlags_Selected;

        bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, gameObject->_children[i]->name.c_str(), i);

        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("GameObject", gameObject->_children[i], sizeof(GameObject*));

            draggingGameObject = gameObject->_children[i];

            ImGui::Text("Change game object parent");
            ImGui::EndDragDropSource();
        }

        if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left))
        {
            layerEditor->SetSelectGameObject(gameObject->_children[i]);
        }

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
            {
                draggingGameObject->SetParent(gameObject->_children[i]);
                draggingGameObject = nullptr;
            }
            ImGui::EndDragDropTarget();
        }

        if (node_open)
        {
            DrawGameObjectChildren(gameObject->_children[i], ++layer);
            ImGui::TreePop();
        }
    }
}
