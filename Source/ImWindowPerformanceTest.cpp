#include "Headers.h"
#include "ImWindowPerformanceTest.h"
#include "MeshImporter.h"
#include "TextureImporter.h"

ImWindowPerformanceTest::ImWindowPerformanceTest()
{
	windowName = "Performance Testing";
}

ImWindowPerformanceTest::~ImWindowPerformanceTest()
{
}

void ImWindowPerformanceTest::Update()
{
	if (ImGui::Begin(windowName.c_str()))
	{
		if (ImGui::Button("Add 10 Bake House models"))
		{
			for (int i = 0; i < 10; i++)
			{
				GameObject* mesh = MeshImporter::LoadMesh("Assets/BakerHouse.fbx");
				mesh->transform->SetPosition({ (float)(rand() % 100), 1.0f, (float)(rand() % 100) });
				std::vector<GameObject*>* children = mesh->GetChildren();
				for (int i = 0; i < children->size(); i++)
				{
					children->at(i)->GetComponent<MeshRenderComponent>()->GetMesh().OpenGLTextureID = TextureImporter::ImportTextureSTBI("Assets/Baker_house.png");
				}
			}
		}
		if (ImGui::Button("Add 100 Bake House models"))
		{
			for (int i = 0; i < 100; i++)
			{
				GameObject* mesh = MeshImporter::LoadMesh("Assets/BakerHouse.fbx");
				mesh->transform->SetPosition({ (float)(rand() % 100), 1.0f, (float)(rand() % 100) });
				std::vector<GameObject*>* children = mesh->GetChildren();
				for (int i = 0; i < children->size(); i++)
				{
					children->at(i)->GetComponent<MeshRenderComponent>()->GetMesh().OpenGLTextureID = TextureImporter::ImportTextureSTBI("Assets/Baker_house.png");
				}
			}
		}
		if (ImGui::Button("Add 1000 Bake House models"))
		{
			for (int i = 0; i < 1000; i++)
			{
				GameObject* mesh = MeshImporter::LoadMesh("Assets/BakerHouse.fbx");
				mesh->transform->SetPosition({ (float)(rand() % 100), 1.0f, (float)(rand() % 100) });
				std::vector<GameObject*>* children = mesh->GetChildren();
				for (int i = 0; i < children->size(); i++)
				{
					children->at(i)->GetComponent<MeshRenderComponent>()->GetMesh().OpenGLTextureID = TextureImporter::ImportTextureSTBI("Assets/Baker_house.png");
				}
			}
		}
	}
	ImGui::End();
}
