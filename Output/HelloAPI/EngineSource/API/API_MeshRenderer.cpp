#include "Headers.h"
#include "API_MeshRenderer.h"
#include "Console.h"
#include "ModuleLayers.h"
#include "MeshRenderComponent.h"
#include "API_Material.h"

API::API_MeshRenderer::API_MeshRenderer()
{
}

API::API_MeshRenderer::~API_MeshRenderer()
{
}

API::API_GameObject API::API_MeshRenderer::GetGameObject()
{
	if (!_meshRenderer)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR MeshRenderer. GetGameObject()");
		return *ModuleLayers::emptyAPIGameObject;
	}
	API_GameObject returnGO;
	returnGO.SetGameObject(_meshRenderer->GetGameObject());
	return returnGO;
}

void API::API_MeshRenderer::ChangeMesh(uint meshResourceUID)
{
	if (!_meshRenderer)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR MeshRenderer. ChangeMesh()");
		return;
	}
	_meshRenderer->CreateMesh(meshResourceUID);
}

void API::API_MeshRenderer::SetActive(bool active)
{
	if (!_meshRenderer)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR MeshRenderer. SetActive()");
		return;
	}
	if (active)
		_meshRenderer->Enable();
	else
		_meshRenderer->Disable();
}

MeshRenderComponent* API::API_MeshRenderer::GetComponent()
{
	return _meshRenderer;
}

void API::API_MeshRenderer::SetComponent(MeshRenderComponent* component)
{
	_meshRenderer = component;
}
