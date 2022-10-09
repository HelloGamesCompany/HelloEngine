#include "Headers.h"
#include "ModelRenderManager.h"
#include "IL/il.h"
#include "IL/ilut.h"

ModelRenderManager::ModelRenderManager()
{
}

ModelRenderManager::~ModelRenderManager()
{
	RELEASE(textureManager);
}

void ModelRenderManager::Init()
{
	ilInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	textureManager = new TextureManager();
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
