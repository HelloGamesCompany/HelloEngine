#include "Headers.h"
#include "API_Scene.h"
#include "ModuleLayers.h"

void API::Scene::LoadScene(const char* path)
{
	std::string fullPath = "Assets/";
	std::string temp(path);
	fullPath += temp;

	ModuleLayers::S_RequestLoadScene(fullPath);
}
