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
		if (selectGameobject && selectGameobject->_parent)
		{
			if (ImGui::SmallButton(selectGameobject->IsActive() ? "X" : " "))
				selectGameobject->SetActive(!selectGameobject->IsActive());
			ImGui::SameLine(); 

			ImGui::InputText("Name", &selectGameobject->name);
			ImGui::InputText("Tag", &selectGameobject->tag);
			
#ifdef STANDALONE
			selectGameobject->OnEditor();
#endif // STANDALONE


		}
	}
	ImGui::End();
}

void ImWindowInspector::SelectGameObject(GameObject* g)
{
	selectGameobject = g;
}