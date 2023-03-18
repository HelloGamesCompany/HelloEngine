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
#endif
	void RootBoneDropArea();

	void UpdateBones(Animation3D* animation = nullptr, float animationTime = 0);

private:
	void LinkBones(GameObject* goBone, std::map<std::string, BoneData>& boneDataMap, Animation3D* animation = nullptr, float4x4 parentTransform = float3x4::identity, float animationTime = 0);

public:
	bool hasAnim = false;
	std::vector<float4x4> goBonesArr;

private:
	GameObject* rootBone = nullptr;

	
};
	
	

	
