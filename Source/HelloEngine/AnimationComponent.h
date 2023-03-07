#pragma once
#include "Component.h"

struct ResourceAnimation;


class Animation;

class AnimationComponent : public Component
{
public:
	AnimationComponent(GameObject* gameObject);
	~AnimationComponent();

	void PlayAnimation();
	void StopAnimation();

	void UpdateAnimation();
	
	void Serialization(json& j) override;
	void DeSerialization(json& j) override;
#ifdef STANDALONE
	void OnEditor() override;

private:
	void AnimationDropArea();

#endif
	
public:

	ResourceAnimation* _resource = nullptr;

	bool isPlaying;
	bool isLoop;
	bool isStayLast;

	float currentKeyframe = 0;
	float animStartPlayTime = 0;
	float animEndPlayTime = 0;
	float currentTime = 0;

	std::vector<float4x4> finalBoneMatrices;
	Animation* currentAnimation = nullptr;
private:

	float CalculateScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
	float3x4 InterpolateMatrix(float3x4 currentMatrix, float3x4 nextMatrix, float animationTime);
};

