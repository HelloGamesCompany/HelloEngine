#pragma once

#include <string>
#include <vector>
#include "Globals.h"
#include "UUID.h"
#include "MathGeoLib.h"

struct AnimatedBone
{
	AnimatedBone() {}
	AnimatedBone(std::string name, int nKeyframes) : name(name)
	{
		keyframes.resize(nKeyframes);

		for (int i = 0; i < nKeyframes; i++) {
			keyframes[i] = float3x4::identity;
		}
	}

	std::string name = "";
	std::vector<float3x4> keyframes;
};

struct Animation3d
{
public:
	Animation3d() {}
	~Animation3d()
	{
		for (int i = 0; i < bones.size(); i++)
		{
			RELEASE(bones[i]);
		}
	}

	uint durationTicks = 0;
	uint ticksPerSecond = 0;
	std::vector<AnimatedBone*> bones;

	/// Returns path to created binary file.
	std::string SaveToBinaryFile(std::string fileName)
	{
		std::string filePath = "Resources/Animations/" + std::to_string(HelloUUID::GenerateUUID()) + ".hanim";
		
		//Get all bones buffer size
		uint bonearraysize = 0;
		for (int i = 0; i < bones.size(); i++)
		{
			bonearraysize += sizeof(uint) + sizeof(bones[i]->name.c_str()) + bones[i]->keyframes.size() * sizeof(float3x4);
		}

		uint header[3] = { durationTicks, ticksPerSecond, bones.size() };

		uint fileSize = sizeof(header) + bonearraysize;
		char* fileBuffer = new char[fileSize];
		char* cursor = fileBuffer;
	
		// Save header
		memcpy(cursor, header, sizeof(header));
		cursor += sizeof(header);

		// Save bones
		for (int i = 0; i < bones.size(); i++)
		{
			//Save string size
			uint stringsize = bones[i]->name.size();
			memcpy(cursor, &stringsize, sizeof(uint));
			cursor += sizeof(uint);

			//Save string
			uint nameSize = sizeof(bones[i]->name.c_str());
			memcpy(cursor, bones[i]->name.c_str(), nameSize);
			cursor += nameSize;
			
			//Save keyframe array
			uint keyframeSize = bones[i]->keyframes.size() * sizeof(float3x4);
			memcpy(cursor, &bones[i]->keyframes[0], keyframeSize);
			//cursor += keyframeSize;
		}

		ModuleFiles::S_Save(filePath, fileBuffer, fileSize, false);

		RELEASE(fileBuffer);

		return filePath;
	}

	void LoadFromBinaryFile(const std::string& filePath)
	{
		char* buffer = nullptr;
		ModuleFiles::S_Load(filePath, &buffer);

		char* cursor = buffer;

		uint header[3];
		uint headerSize = sizeof(uint) * 4;
		memcpy(header, cursor, headerSize);
		cursor += headerSize;

		durationTicks = header[0];
		ticksPerSecond = header[1];
		bones.resize(header[2]);

		for (int i = 0; i < header[2]; i++)
		{
			AnimatedBone* bone = new AnimatedBone();

			uint stringsize;
			memcpy(&stringsize, cursor, sizeof(uint));
			cursor += sizeof(uint);

			bone->name.resize(stringsize);
			memcpy(&bone->name[0], cursor, stringsize);
			cursor += stringsize;

			bone->keyframes.resize(durationTicks + 1);
			memcpy(&bone->keyframes[0], cursor, (durationTicks + 1) * sizeof(float3x4));
			//cursor += (durationTicks + 1) * sizeof(float3x4);
		}

		RELEASE(buffer);
	}
};