#pragma once

#include "RenderManager.h"

/// <summary>
/// This class contains a colletion of RenderManagers. It dynamically creates and destroys Render Managers tu fullfill the task of having one per Unique mesh.
/// Every Render Manager updates and draws their corresponding Models.
/// </summary>
class ModelRenderManager
{
public:
	ModelRenderManager();
	~ModelRenderManager();

	RenderManager* GetRenderManager(uint ID);
	uint GetMapSize() { return renderMap.size(); };

	void Draw();

private:
	std::map<uint, RenderManager> renderMap;
};

