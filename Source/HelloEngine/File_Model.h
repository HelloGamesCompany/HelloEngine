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
	}

	/// <summary>
	/// Returns path to created binary file.
	/// </summary>
	std::string SaveToBinaryFile(std::string fileName)
	{
		std::string filePath = "Resources/Meshes/" + std::to_string(HelloUUID::GenerateUUID()) + ".hmesh";

		//Bone map to vectors
		std::vector<std::string> boneMapKeys;
		std::vector<BoneData> boneMapValues;
		for (std::map<std::string, BoneData>::iterator it = boneDataMap.begin(); it != boneDataMap.end(); ++it) {
			boneMapKeys.push_back(it->first);
			boneMapValues.push_back(it->second);
		}
		uint stringSize = 0;
		for (int i = 0; i < boneMapKeys.size(); ++i)
		{
			stringSize += sizeof(char) * boneMapKeys[i].size();
		}

		uint verticesSize = vertices.size() * sizeof(Vertex);
		uint indicesSize = indices.size() * sizeof(uint);
		uint boneMapKeysSize = boneMapKeys.size() * sizeof(char);
		uint boneMapValuesSize = boneMapValues.size() * sizeof(BoneData);
		
		uint header[4] = { vertices.size(), indices.size(), stringSize, boneMapValues.size() }; // Num of vertices, num of indices, has or not a texture.

		uint fileSize = verticesSize + indicesSize + stringSize + boneMapValuesSize; // Vertex + indices + boneMapKeysSize + boneMapKeysValue

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
		if (stringSize != 0)
		{
			//Save Keys of boneDataMap
			memcpy(cursor, &boneMapKeys[0], stringSize);
			cursor += stringSize;

			//Save Values of boneDataMap
			memcpy(cursor, &boneMapValues[0], boneMapValuesSize);
			cursor += boneMapValuesSize;
		}	

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

		uint boneMapKeysSize = header[2] * sizeof(std::string);
		if (boneMapKeysSize != 0)
		{
			boneMapKeys.resize(header[2]);
			memcpy(&boneMapKeys[0], cursor, boneMapKeysSize);
			cursor += boneMapKeysSize;

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