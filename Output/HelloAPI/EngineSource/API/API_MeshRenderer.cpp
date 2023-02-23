#include "Headers.h"
#include "API_MeshRenderer.h"
#include "Console.h"
#include "ModuleLayers.h"
#include "MeshRenderComponent.h"

API::API_MeshRenderer::API_MeshRenderer()
{
}

API::API_MeshRenderer::~API_MeshRenderer()
{
}

API::API_GameObject API::API_MeshRenderer::GetGameObject()
{
	if (_meshRenderer == nullptr)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR GameObject. GetTransform()");
		return *ModuleLayers::emptyAPIGameObject;
	}
	API_GameObject returnGO;
	returnGO.SetGameObject(_meshRenderer->GetGameObject());
	return returnGO;
}

MeshRenderComponent* API::API_MeshRenderer::GetComponent()
{
	return _meshRenderer;
}

void API::API_MeshRenderer::SetComponent(MeshRenderComponent* component)
{
	_meshRenderer = component;
}
