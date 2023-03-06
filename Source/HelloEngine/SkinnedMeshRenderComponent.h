#pragma once
#include "MeshRenderComponent.h"
class SkinnedMeshRenderComponent : public MeshRenderComponent
{
public:
	SkinnedMeshRenderComponent(GameObject* gameObject);
	SkinnedMeshRenderComponent(GameObject* gameObject, const SkinnedMeshRenderComponent& copy);
	~SkinnedMeshRenderComponent();

	void Serialization(json& j) override;
	void DeSerialization(json& j) override;

	void CreateMesh(uint resourceUID, MeshRenderType type = MeshRenderType::INDEPENDENT) override;

#ifdef STANDALONE
	void OnEditor() override;

	void RootBoneDropArea();
#endif // STANDALONE

	void UpdateBones();

private:
	void LinkBones(GameObject* goBone, std::map<std::string, BoneData> boneDataMap);

public:

	std::vector<float4x4> goBonesArr;

private:
	GameObject* rootBone = nullptr;

	
};
	
	

	
