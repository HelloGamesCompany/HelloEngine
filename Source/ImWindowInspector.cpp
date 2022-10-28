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
			ImGui::InputText("Name", &selectGameobject->name,0);

			selectGameobject->OnEditor();
		}
	}
	ImGui::End();
}

void ImWindowInspector::SelectGameObject(GameObject* g)
{
	selectGameobject = g;
}