#include "Headers.h"
#include "ImWindowInspector.h"
#include "GameObject.h"
#include "TransformComponent.h"

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

			if(ImGui::CollapsingHeader("Transform"))
			{
				float3 tempPos = selectGameobject->GetComponent<TransformComponent>()->localTransform.position;
				float3 tempRot = selectGameobject->GetComponent<TransformComponent>()->localTransform.rotation;
				float3 tempScale = selectGameobject->GetComponent<TransformComponent>()->localTransform.scale;

				if (ImGui::DragFloat3("position", &tempPos[0], 0.1f))
				{
					selectGameobject->GetComponent<TransformComponent>()->SetPosition(tempPos);
				}

				if (ImGui::DragFloat3("rotation", &tempRot[0], 0.1f))
				{
					selectGameobject->GetComponent<TransformComponent>()->SetRotation(tempRot);
				}

				if (ImGui::DragFloat3("scale", &tempScale[0], 0.1f))
				{
					selectGameobject->GetComponent<TransformComponent>()->SetScale(tempScale);
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