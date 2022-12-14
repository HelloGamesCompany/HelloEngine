#include "Headers.h"
#include "ImWindowPerformanceTest.h"
#include "MeshImporter.h"
#include "TextureImporter.h"
#include "ModuleResourceManager.h"

ImWindowPerformanceTest::ImWindowPerformanceTest()
{
	windowName = "Performance Testing";
}

ImWindowPerformanceTest::~ImWindowPerformanceTest()
{
}

void ImWindowPerformanceTest::Update()
{
	static int houseNum = 2;
	if (ImGui::Begin(windowName.c_str()))
	{
		/*if (ImGui::Button("Add 10 Baker House models"))
		{
			houseNum += 10;
			for (int i = 0; i < 10; i++)
			{
				GameObject* mesh = MeshImporter::LoadMesh("Assets/FBX/BakerHouse.fbx");
				mesh->transform->SetPosition({ (float)(rand() % 100), 1.0f, (float)(rand() % 100) });
			}
		}
		if (ImGui::Button("Add 100 Baker House models"))
		{
			houseNum += 100;
			for (int i = 0; i < 100; i++)
			{
				GameObject* mesh = MeshImporter::LoadMesh("Assets/FBX/BakerHouse.fbx");
				mesh->transform->SetPosition({ (float)(rand() % 100), 1.0f, (float)(rand() % 100) });
			}
		}
		if (ImGui::Button("Add 1000 Baker House models"))
		{
			houseNum += 1000;
			for (int i = 0; i < 1000; i++)
			{
				GameObject* mesh = MeshImporter::LoadMesh("Assets/FBX/BakerHouse.fbx");
				mesh->transform->SetPosition({ (float)(rand() % 100), 1.0f, (float)(rand() % 100) });
			}
		}*/
		ImGui::TextWrapped("Number of houses: %d", houseNum);
	}
	ImGui::End();
}
