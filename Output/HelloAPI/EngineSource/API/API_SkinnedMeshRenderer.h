#pragma once
#include "Globals.h"
#include "API_GameObject.h"

class SkinnedMeshRenderComponent;

namespace API
{
	class TO_API API_SkinnedMeshRenderer
	{
	public:
		API_SkinnedMeshRenderer();
		~API_SkinnedMeshRenderer();

		API_GameObject GetGameObject();

		void SetRootBone(API_GameObject rootBone);

	private:
		SkinnedMeshRenderComponent* GetComponent();
		void SetComponent(SkinnedMeshRenderComponent* component);
	private:
		SkinnedMeshRenderComponent* _skinnedMeshRenderer = nullptr;

		friend class API_GameObject;
		friend class SkinnedMeshRenderComponent;
	};
}

