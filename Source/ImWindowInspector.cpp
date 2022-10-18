#include "Headers.h"
#include "ImWindowInspector.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "MeshRenderComponent.h"

ImWindowInspector::ImWindowInspector()
{
	windowName = "Inspector";

	isEnabled = true;
}

ImWindowInspector::~ImWindowInspector()
{
}

void ImWindowInspector::Update()
{
	if (ImGui::Begin(windowName.c_str(), &isEnabled))
	{
		if (selectGameobject)
		{
			//ImGui::Text(selectGameobject->name.c_str());

			ImGui::InputText("Name", &selectGameobject->name,0);

			std::vector<Component*> components = selectGameobject->GetComponents();

			// TODO: Deberiamos hacer esto dentro de Component como método virtual?

			for (size_t i = 0; i < components.size(); i++)
			{
				switch (components[i]->GetType())
				{
				case Component::Type::TRANSFORM:
					if (ImGui::CollapsingHeader("Transform",ImGuiTreeNodeFlags_DefaultOpen))
					{
						float3 tempPos = selectGameobject->GetComponent<TransformComponent>()->localTransform.position;
						float3 tempRot = selectGameobject->GetComponent<TransformComponent>()->localTransform.rotation;
						float3 tempScale = selectGameobject->GetComponent<TransformComponent>()->localTransform.scale;
						
						{
							ImGui::DragFloat3("position", &tempPos[0], 0.1f);

							if (ImGui::IsItemActivated())
							{
								static float positionX = 0;
								positionX = tempPos.x;
								std::cout << "A= " << positionX << std::endl;
							}
							selectGameobject->GetComponent<TransformComponent>()->SetPosition(tempPos);
						}
						
						
						{
							ImGui::DragFloat3("rotation", &tempRot[0], 0.1f);
							selectGameobject->GetComponent<TransformComponent>()->SetRotation(tempRot); // TODO: Esto se llama cada frame, deberia ser solo cuando cambia el valor.
						}

						{
							ImGui::DragFloat3("scale", &tempScale[0], 0.1f);
							selectGameobject->GetComponent<TransformComponent>()->SetScale(tempScale);
						}
					}
					break;
				case Component::Type::MESH_RENDERER:
					if (ImGui::CollapsingHeader("Mesh Renderer", ImGuiTreeNodeFlags_DefaultOpen))
					{
					}
					break;
				}
			}		
		}
	}
	ImGui::End();
}

void ImWindowInspector::SelectGameObject(GameObject* g)
{
	selectGameobject = g;
}