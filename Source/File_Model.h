#pragma once

#include <string>
#include <vector>
#include "Math/float4x4.h"
#include "ModuleXML.h"
#include "Mesh.h"

struct ModelNode
{
	std::string name = "Node";
	std::string meshPath = "N"; // By default, this path is set to "N". Paths with "N" will be seen as Null.
	float3 position = float3::zero;
	float3 rotation = float3::zero;
	float3 scale = {1,1,1};
	int childrenNum = 0;
	std::vector<ModelNode> children;

	void WriteToXML(std::string nodeName, pugi::xml_node& whereToWrite)
	{
		name = nodeName;
		pugi::xml_node xmlNode = whereToWrite.append_child(name.c_str());

		// Save Mesh path
		pugi::xml_node meshPathNode = xmlNode.append_child("MeshPath");
		pugi::xml_attribute meshPathAtt = meshPathNode.append_attribute("Path");
		meshPathAtt.set_value(meshPath.c_str());

		// Save transform (it has to be done variable per variable)
		// Position 
		pugi::xml_node positionNode = xmlNode.append_child("Position");
		pugi::xml_attribute positionAttX = positionNode.append_attribute("X");
		positionAttX.set_value(position.x);

		pugi::xml_attribute positionAttY = positionNode.append_attribute("Y");
		positionAttY.set_value(position.y);

		pugi::xml_attribute positionAttZ = positionNode.append_attribute("Z");
		positionAttZ.set_value(position.z);

		// Rotation
		pugi::xml_node rotationNode = xmlNode.append_child("Rotation");
		pugi::xml_attribute rotationAttX = rotationNode.append_attribute("X");
		rotationAttX.set_value(rotation.x);

		pugi::xml_attribute rotationAttY = rotationNode.append_attribute("Y");
		rotationAttY.set_value(rotation.y);

		pugi::xml_attribute rotationAttZ = rotationNode.append_attribute("Z");
		rotationAttZ.set_value(rotation.z);

		// Scale
		pugi::xml_node scaleNode = xmlNode.append_child("Scale");
		pugi::xml_attribute scaleAttX = scaleNode.append_attribute("X");
		scaleAttX.set_value(scale.x);

		pugi::xml_attribute scaleAttY = scaleNode.append_attribute("Y");
		scaleAttY.set_value(scale.y);

		pugi::xml_attribute scaleAttZ = scaleNode.append_attribute("Z");
		scaleAttZ.set_value(scale.z);

		// Save num of children
		pugi::xml_node childrenNode = xmlNode.append_child("ChildrenNum");
		pugi::xml_attribute childrenAtt = childrenNode.append_attribute("Value");
		childrenAtt.set_value(children.size());

		// Save Children
		for (int i = 0; i < children.size(); i++)
		{
			children[i].WriteToXML("Node" + std::to_string(i), xmlNode);
		}
	}

	/// <summary>
	/// Reads the Model node data from an XMLNode
	/// </summary>
	/// <param name="nodeName">Name of the node you are reading from.</param>
	/// <param name="whereToRead">XMLNode to the root node of the XML file.</param>
	void ReadFromXML(std::string nodeName, XMLNode& whereToRead)
	{
		name = nodeName;
		XMLNode xmlNode = whereToRead.FindChildBreadth(name);

		meshPath = xmlNode.FindChildBreadth("MeshPath").node.attribute("Path").as_string();


		pugi::xml_node positionNode = xmlNode.FindChildBreadth("Position").node;
		position.x = positionNode.attribute("X").as_float();
		position.y = positionNode.attribute("Y").as_float();
		position.z = positionNode.attribute("Z").as_float();

		pugi::xml_node rotationNode = xmlNode.FindChildBreadth("Rotation").node;
		rotation.x = rotationNode.attribute("X").as_float();
		rotation.y = rotationNode.attribute("Y").as_float();
		rotation.z = rotationNode.attribute("Z").as_float();

		pugi::xml_node scaleNode = xmlNode.FindChildBreadth("Scale").node;
		scale.x = scaleNode.attribute("X").as_float();
		scale.y = scaleNode.attribute("Y").as_float();
		scale.z = scaleNode.attribute("Z").as_float();

		childrenNum = xmlNode.FindChildBreadth("ChildrenNum").node.attribute("Value").as_int();

		for (int i = 0; i < childrenNum; i++)
		{
			ModelNode newNode;
			newNode.ReadFromXML("Node" + std::to_string(i), xmlNode);
			children.push_back(newNode);
		}
	}
};

struct MeshInfo
{
	std::vector<Vertex> vertices;
	std::vector<uint> indices;
	uint hasTexture = 0; // For now, this only implies a diffuse texture.

	/// <summary>
	/// Returns path to created binary file.
	/// </summary>
	std::string SaveToBinaryFile(std::string fileName)
	{
		std::string filePath = "Resources/Meshes/" + fileName + ".hmsh";

		uint header[3] = { vertices.size(), indices.size(), hasTexture }; // Num of vertices, num of indices, has or not a texture.

		uint verticesSize = vertices.size() * sizeof(Vertex);
		uint indicesSize = indices.size() * sizeof(uint);

		uint fileSize = verticesSize + indicesSize + sizeof(uint); // Vertex + indices + hasTexture.

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

		ModuleFiles::S_Save(filePath, fileBuffer, fileSize, false);

		RELEASE(fileBuffer);

		return filePath;
	}

	void LoadFromBinaryFile(std::string filePath)
	{
		char* buffer = nullptr;
		uint size = ModuleFiles::S_Load(filePath, &buffer);

		char* cursor = buffer;

		uint header[3];
		uint headerSize = sizeof(uint) * 3;
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

		hasTexture = header[2];

		RELEASE(buffer);
	}
};