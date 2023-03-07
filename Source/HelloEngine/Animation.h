#pragma once

#include "AnimationBone.h"

class SkinnedMeshRenderer;
class aiAnimation;

struct AssimpNodeData
{
	float4x4 transformation;
	std::string name;
	int childrenCount;
	std::vector<AssimpNodeData> children;
};

class Animation
{
public:
	Animation() = default;
	~Animation() {};

	AnimationBone* FindBone(std::string name);
	AnimationBone FindBoneByArrayIndex(int i);

	inline float GetTicksPerSecond() { return ticksPerSecond; }
	inline float GetDuration() { return duration; }
	
public:
	void ImportAnimation(aiAnimation* animation);
	std::vector<AnimationBone> bones;
private:
	float duration;
	int ticksPerSecond;
	
	AssimpNodeData m_RootNode;
	//std::map<std::string, BoneInfo> m_BoneInfoMap;
};

