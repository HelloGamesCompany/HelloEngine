#include "Headers.h"
#include "ModelRenderManager.h"

ModelRenderManager::ModelRenderManager()
{
}

ModelRenderManager::~ModelRenderManager()
{
}

RenderManager* ModelRenderManager::GetRenderManager(uint ID)
{
	return &renderMap[ID];
}

void ModelRenderManager::Draw()
{
	for (auto& obj : renderMap)
	{
		obj.second.Draw();
	}
}
