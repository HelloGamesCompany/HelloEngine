#pragma once

#include <string>
#include <vector>
#include "Math/float4x4.h"
#include "ModuleXML.h"
#include "Mesh.h"
#include "json.hpp"

using json = nlohmann::json;

struct ModelNode
{
	std::string name = "Node";
	std::string meshPath = "N"; // By default, this path is set to "N". Paths with "N" will be seen as Null.
	uint resourceMaterialUID = 0;
	float3 position = float3::zero;
	float3 rotation = float3::zero;
	float3 scale = {1,1,1};
	int childrenNum = 0;
	std::vector<ModelNode> children;
	uint UID = 0;

	void Reset()
	{
		UID = 0;
		children.clear();
		childrenNum = 0;
		position = float3::zero;
		rotation = float3::zero;
		scale = { 1,1,1 };
		resourceMaterialUID = 0;
		name = "Node";
		meshPath = "N";
	}

	void WriteToJSON(std::string filename = "")
	{
		json file;

		WriteToParent(file, 0, true);

		std::string data = file.dump();

		ModuleFiles::S_Save(filename, &data[0], data.size(), false);
	}

	void ReadFromJSON(std::string filePath)
	{
		char* data = nullptr;
		ModuleFiles::S_Load(filePath, &data);

		if (data == nullptr)
			return;

		json file = json::parse(data);

		std::vector<std::string> childrenUIDs = file["0"]["children"];

		name = file["0"]["name"];
		meshPath = file["0"]["meshPath"];
		resourceMaterialUID = file["0"]["resourceUID"];

		std::vector<float> positions = file["0"]["position"];
		position = { positions[0], positions[1], positions[2] };

		std::vector<float> rotations = file["0"]["rotation"];
		rotation = { rotations[0], rotations[1], rotations[2] };

		std::vector<float> scales = file["0"]["scale"];
		scale = { scales[0], scales[1], scales[2] };

		UID = 0;
		childrenNum = childrenUIDs.size();

		for (int i = 0; i < childrenUIDs.size(); i++)
		{
			ModelNode newNode;
			newNode.ReadFromParent(file, childrenUIDs[i]);
			children.push_back(newNode);
		}
		RELEASE(data);
	}

private:
	std::string WriteToParent(json& file, uint parentUID, bool isRoot = false)
	{
		if (!isRoot)
			UID = HelloUUID::GenerateUUID();
		else
			UID = 0;

		std::string stringUID = std::to_string(UID);

		file[stringUID]["name"] = name;
		file[stringUID]["meshPath"] = meshPath;
		file[stringUID]["position"] = { position.x, position.y, position.z };
		file[stringUID]["rotation"] = { rotation.x, rotation.y, rotation.z };
		file[stringUID]["scale"] = { scale.x, scale.y, scale.z };
		file[stringUID]["childrenNum"] = children.size();
		file[stringUID]["UID"] = UID;
		file[stringUID]["parentUID"] = parentUID;
		file[stringUID]["resourceUID"] = resourceMaterialUID;

		std::vector<std::string> childrenUIDs; 

		for (int i = 0; i < children.size(); i++)
		{
			childrenUIDs.push_back(children[i].WriteToParent(file, UID));
		}
		file[stringUID]["children"] = childrenUIDs;
		return stringUID;
	}

	void ReadFromParent(json& file, std::string uid)
	{
		std::vector<std::string> childrenUIDs = file[uid]["children"];

		name = file[uid]["name"];
		meshPath = file[uid]["meshPath"];
		resourceMaterialUID = file[uid]["resourceUID"];

		std::vector<float> positions = file[uid]["position"];
		position = { positions[0], positions[1], positions[2] };

		std::vector<float> rotations = file[uid]["rotation"];
		rotation = { rotations[0], rotations[1], rotations[2] };

		std::vector<float> scales = file[uid]["scale"];
		scale = { scales[0], scales[1], scales[2] };

		UID = std::stoul(uid);
		childrenNum = childrenUIDs.size();

		for (int i = 0; i < childrenUIDs.size(); i++)
		{
			ModelNode newNode;
			newNode.ReadFromParent(file, childrenUIDs[i]);
			children.push_back(newNode);
		}
	}
};

struct MeshInfo
{
	std::vector<Vertex> vertices;
	std::vector<uint> indices;
	uint hasTexture = 0; // For now, this only implies a diffuse texture.
	std::map<std::string, BoneData> boneDataMap;


	void Clear()
	{
		vertices.clear();
		indices.clear();
		boneDataMap.clear();
	}

	/// <summary>
	/// Returns path to created binary file.
	/// </summary>
	std::string SaveToBinaryFile(std::string fileName)
	{
		std::string filePath = "Resources/Meshes/" + std::to_string(HelloUUID::GenerateUUID()) + ".hmesh";

		std::string corruptionPrev;
		corruptionPrev.resize(128);

		//Bone map to vectors
		std::string fullMapKeyString = "";
		std::vector<BoneData> boneMapValues;
		for (std::map<std::string, BoneData>::iterator it = boneDataMap.begin(); it != boneDataMap.end(); ++it) {
			//All bone map keys into one long string
			fullMapKeyString += it->first;
			fullMapKeyString += "\n";

			boneMapValues.push_back(it->second);
		}

		uint verticesSize = vertices.size() * sizeof(Vertex);
		uint indicesSize = indices.size() * sizeof(uint);
		uint boneMapValuesSize = boneMapValues.size() * sizeof(BoneData);
		
		uint header[4] = { vertices.size(), indices.size(), fullMapKeyString.size(), boneMapValues.size()};

		uint fileSize = verticesSize + indicesSize + fullMapKeyString.size() + boneMapValuesSize + corruptionPrev.size(); // Vertex + indices + boneMapKeysSize + boneMapKeysValue

		char* fileBuffer = new char[fileSize];
		char* cursor = fileBuffer;

		// Save header
		uint headerSize = sizeof(header);
		memcpy(cursor, header, headerSize);
		cursor += headerSize;

		// Save indices
		memcpy(cursor, &indices[0], indicesSize);
		cursor += indicesSize;

		// Save vertices
		memcpy(cursor, &vertices[0], verticesSize);
		cursor += verticesSize;

		//Save boneDataMap((
		if (fullMapKeyString.size() != 0)
		{
			//Save Keys of boneDataMap
			memcpy(cursor, &fullMapKeyString[0], fullMapKeyString.size());
			cursor += fullMapKeyString.size();

			//Save Values of boneDataMap
			memcpy(cursor, &boneMapValues[0], boneMapValuesSize);
			cursor += boneMapValuesSize;
		}	

		//Data corruption prevention
		// Save vertices
		memcpy(cursor, &corruptionPrev[0], corruptionPrev.size());
		cursor += corruptionPrev.size();

		ModuleFiles::S_Save(filePath, fileBuffer, fileSize, false);

		RELEASE(fileBuffer);

		return filePath;
	}

	void LoadFromBinaryFile(const std::string& filePath)
	{
		std::vector<std::string> boneMapKeys;
		std::vector<BoneData> boneMapValues;

		char* buffer = nullptr;
		uint size = ModuleFiles::S_Load(filePath, &buffer);

		char* cursor = buffer;

		uint header[4];
		uint headerSize = sizeof(uint) * 4;
		memcpy(header, cursor, headerSize);
		cursor += headerSize;

		uint indicesSize = header[1] * sizeof(uint);
		indices.resize(header[1]);
		memcpy(&indices[0], cursor, indicesSize);
		cursor += indicesSize;

		uint verticesSize = header[0] * sizeof(Vertex);
		vertices.resize(header[0]);
		memcpy(&vertices[0], cursor, verticesSize);
		cursor += verticesSize;

		uint boneMapKeysSize = header[2] * sizeof(char);
		if (boneMapKeysSize != 0)
		{
			std::string fullMapKeyString;
			fullMapKeyString.resize(header[2]);
			memcpy(&fullMapKeyString[0], cursor, boneMapKeysSize);
			cursor += boneMapKeysSize;

			//fullMapKeyString into boneMapKeys (Vector)
			std::stringstream ss(fullMapKeyString);
			std::string tmp;
			while (std::getline(ss, tmp, '\n')) {
				boneMapKeys.emplace_back(tmp);
			}


			uint boneMapValuesSize = header[3] * sizeof(BoneData);
			boneMapValues.resize(header[3]);
			memcpy(&boneMapValues[0], cursor, boneMapValuesSize);
			cursor += boneMapValuesSize;
		

			//BoneMap vectors to map.
			for (int i = 0; i < boneMapKeys.size(); ++i)
			{
				boneDataMap.emplace(boneMapKeys[i], boneMapValues[i]);
			}
		}

		RELEASE(buffer);
	}
};

struct AnimatedBone
{
	/*AnimatedBone() {}
	AnimatedBone(std::string name, int nKeyframes) : name(name)
	{
		keyframes.resize(nKeyframes);

		for (int i = 0; i < nKeyframes; i++) {
			keyframes[i] = float3x4::identity;
		}
	}*/

	void SizeKeyframes(int nKeyframes)
	{
		keyframes.resize(nKeyframes);

		for (int i = 0; i < nKeyframes; i++) {
			keyframes[i] = float3x4::identity;
		}
	}

	std::string name = "";
	std::vector<float3x4> keyframes;
};

struct Animation3D
{
public:
	uint durationTicks = 0;
	uint ticksPerSecond = 0;
	std::vector<AnimatedBone> bones;

	void Clear()
	{
		bones.clear();
	}

	/// Returns path to created binary file.
	std::string SaveToBinaryFile()
	{
		std::string filePath = "Resources/Animations/" + std::to_string(HelloUUID::GenerateUUID()) + ".hanim";

		//Get all bones buffer size
		uint bonearraysize = 0;
		for (int i = 0; i < bones.size(); i++)
		{
			bonearraysize += sizeof(uint) + sizeof(bones[i].name.c_str()) + bones[i].keyframes.size() * sizeof(float3x4);
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
			uint stringsize = bones[i].name.size();
			memcpy(cursor, &stringsize, sizeof(uint));
			cursor += sizeof(uint);

			//Save string
			uint nameSize = sizeof(bones[i].name.c_str());
			memcpy(cursor, bones[i].name.c_str(), nameSize);
			cursor += nameSize;

			//Save keyframe array
			uint keyframeSize = bones[i].keyframes.size() * sizeof(float3x4);
			memcpy(cursor, &bones[i].keyframes[0], keyframeSize);
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