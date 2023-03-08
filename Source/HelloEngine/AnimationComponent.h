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
	void ResumeAnimation();
	void PauseAnimation();

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
	bool isPaused;
	bool isLoop;
	bool isStayLast;

	float animDuration = 0;
	float currentTime = 0;
	float speedMultiplier = 1;

	std::vector<float4x4> finalBoneMatrices;
	Animation* currentAnimation = nullptr;
private:

	float CalculateScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
	float3x4 InterpolateMatrix(float3x4 currentMatrix, float3x4 nextMatrix, float animationTime);
};

