#pragma once

class aiNodeAnim;

struct KeyPosition
{
	float3 position;
	float timeStamp;
};

struct KeyRotation
{
	Quat orientation;
	float timeStamp;
};

struct KeyScale
{
	float3 scale;
	float timeStamp;
};

class AnimationBone
{
private:
	std::vector <KeyPosition> positions;
	std::vector <KeyRotation> rotations;
	std::vector <KeyScale> scales;
	int numPositions;
	int numRotations;
	int numScaling;

	float4x4 localTransform;
	std::string name;
	int id;

public:
	AnimationBone(const std::string name, int id, aiNodeAnim* channel);
	~AnimationBone() {};


	float4x4 Update(float animationTime);

	float4x4 GetLocalTransform() { return localTransform; }
	std::string GetBoneName() const { return name; }
	int GetBoneID() { return id; }

	int GetPositionIndex(float animationTime);
	int GetRotationIndex(float animationTime);
	int GetScalingIndex(float animationTime);

private:

	float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);

	float4x4 InterpolatePosition(float animationTime);
	float4x4 InterpolateRotation(float animationTime);
	float4x4 InterpolateScaling(float animationTime);
};

