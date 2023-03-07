#include "Headers.h"
#include "AnimationBone.h"

#include "assimp/scene.h"

AnimationBone::AnimationBone(const std::string name, int id, aiNodeAnim* channel)
{
	this->name = name;
	this->id = id;
	this->localTransform = float4x4::identity;

	numPositions = channel->mNumPositionKeys;
	//Positions
	for (int i = 0; i < numPositions; ++i)
	{
		aiVector3D aiPosition = channel->mPositionKeys[i].mValue;
		float timeStamp = channel->mPositionKeys[i].mTime;

		KeyPosition data;
		data.position = float3(aiPosition.x, aiPosition.y, aiPosition.z);
		data.timeStamp = timeStamp;

		positions.push_back(data);
	}

	numRotations = channel->mNumRotationKeys;
	//Rotations
	for (int i = 0; i < numRotations; ++i)
	{
		aiQuaternion aiQuat = channel->mRotationKeys[i].mValue;
		float timeStamp = channel->mRotationKeys[i].mTime;

		KeyRotation data;
		data.orientation = Quat(aiQuat.x, aiQuat.y, aiQuat.z, aiQuat.w);
		data.timeStamp = timeStamp;

		rotations.push_back(data);
	}

	numScaling = channel->mNumScalingKeys;
	//Scaling
	for (int i = 0; i < numScaling; ++i)
	{
		aiVector3D aiScale = channel->mScalingKeys[i].mValue;
		float timeStamp = channel->mScalingKeys[i].mTime;

		KeyScale data;
		data.scale = float3(aiScale.x, aiScale.y, aiScale.z);
		data.timeStamp = timeStamp;

		scales.push_back(data);
	}
}

float4x4 AnimationBone::Update(float animationTime)
{
	float4x4 translation = InterpolatePosition(animationTime);
	float4x4 rotation = InterpolateRotation(animationTime);
	float4x4 scale = InterpolateScaling(animationTime);
	localTransform = translation + rotation + scale;
	return localTransform;
}

float4x4 AnimationBone::InterpolatePosition(float animationTime)
{
	if (1 == numPositions)
		return float4x4::Translate(positions[0].position);

	int i0 = GetPositionIndex(animationTime);
	int i1 = i0 + 1;

	float scaleFactor = GetScaleFactor(positions[i0].timeStamp, positions[i1].timeStamp, animationTime);

	float3 finalPosition = math::Lerp(positions[i0].position, positions[i1].position, scaleFactor);

	return float4x4::Translate(finalPosition);
}

float4x4 AnimationBone::InterpolateRotation(float animationTime)
{
	if (1 == numRotations)
	{
		Quat rotation = rotations[0].orientation.Normalized();
		return rotation.ToFloat4x4();
	}

	int i0 = GetRotationIndex(animationTime);
	int i1 = i0 + 1;

	float scaleFactor = GetScaleFactor(rotations[i0].timeStamp, rotations[i1].timeStamp, animationTime);

	Quat finalRotation = math::Slerp(rotations[i0].orientation, rotations[i1].orientation, animationTime);

	return finalRotation.ToFloat4x4();
}

float4x4 AnimationBone::InterpolateScaling(float animationTime)
{
	if (1 == numScaling)
		return float4x4::Scale(scales[0].scale);

	int i0 = GetScalingIndex(animationTime);
	int i1 = i0 + 1;

	float scaleFactor = GetScaleFactor(scales[i0].timeStamp, scales[i1].timeStamp, animationTime);

	float3 finalScale = math::Lerp(scales[i0].scale, scales[i1].scale, animationTime);

	return float4x4::Scale(finalScale);
}

int AnimationBone::GetPositionIndex(float animationTime)
{
	for (int i = 0; i < numPositions - 1; ++i)
	{
		if (animationTime < positions[i + 1].timeStamp) return i;
	}
	assert(0);
}

int AnimationBone::GetRotationIndex(float animationTime)
{
	for (int i = 0; i < numRotations - 1; ++i)
	{
		if (animationTime < rotations[i + 1].timeStamp) return i;
	}
	assert(0);
}

int AnimationBone::GetScalingIndex(float animationTime)
{
	for (int i = 0; i < numScaling - 1; ++i)
	{
		if (animationTime < scales[i + 1].timeStamp) return i;
	}
	assert(0);
}

float AnimationBone::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
{
	float scaleFactor = 0.0f;
	float midWayLength = animationTime - lastTimeStamp;
	float framesDiff = nextTimeStamp - lastTimeStamp;
	
	scaleFactor = midWayLength / framesDiff;

	return scaleFactor;
}