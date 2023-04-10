#include "Headers.h"
#include "API_SkinnedMeshRenderer.h"
#include "ModuleLayers.h"
#include "SkinnedMeshRenderComponent.h"

API::API_SkinnedMeshRenderer::API_SkinnedMeshRenderer()
{
}

API::API_SkinnedMeshRenderer::~API_SkinnedMeshRenderer()
{
}

API::API_GameObject API::API_SkinnedMeshRenderer::GetGameObject()
{
	if (_skinnedMeshRenderer == nullptr)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR GameObject. GetTransform()");
		return *ModuleLayers::emptyAPIGameObject;
	}

	API_GameObject goToReturn;
	goToReturn.SetGameObject(_skinnedMeshRenderer->GetGameObject());

	return goToReturn;
}

void API::API_SkinnedMeshRenderer::SetRootBone(API_GameObject rootBone)
{
	if (_skinnedMeshRenderer == nullptr || !rootBone.IsAlive())
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR SkinnedMeshRender. SetRootBone()");
		return;
	}

	_skinnedMeshRenderer->SetRootBone(rootBone.GetGameObject());

}

SkinnedMeshRenderComponent* API::API_SkinnedMeshRenderer::GetComponent()
{
    return _skinnedMeshRenderer;
}

void API::API_SkinnedMeshRenderer::SetComponent(SkinnedMeshRenderComponent* component)
{

    _skinnedMeshRenderer = component;

}
