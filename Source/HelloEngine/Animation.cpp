#include "Headers.h"
#include "Animation.h"

#include "assimp/scene.h"

AnimationBone* Animation::FindBone(std::string name)
{
	auto iter = std::find_if(bones.begin(), bones.end(), [&](const AnimationBone& bone)
		{
			return bone.GetBoneName() == name;
		}
	);

	if (iter == bones.end()) return nullptr;
	else return &(*iter);
}

AnimationBone Animation::FindBoneByArrayIndex(int i)
{
	return bones[i];
}

void Animation::ImportAnimation(aiAnimation* animation)
{
	int size = animation->mNumChannels;

	ticksPerSecond = animation->mTicksPerSecond;
	duration = animation->mDuration;

	for (int i = 0; i < size; ++i)
	{
		auto channel = animation->mChannels[i];
		std::string boneName = channel->mNodeName.data;

		bones.push_back(AnimationBone(channel->mNodeName.data, 0, channel));
	}
}