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

		// Save Children
		for (int i = 0; i < children.size(); i++)
		{
			children[i].WriteToXML(children[i].name, xmlNode);
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

		meshPath = whereToRead.FindChildBreadth("MeshPath").node.attribute("Path").as_string();

		position.x = xmlNode.FindChildBreadth("Position").node.attribute("X").as_float();
		position.y = xmlNode.FindChildBreadth("Position").node.attribute("Y").as_float();
		position.z = xmlNode.FindChildBreadth("Position").node.attribute("Z").as_float();

		rotation.x = xmlNode.FindChildBreadth("Rotation").node.attribute("X").as_float();
		rotation.y = xmlNode.FindChildBreadth("Rotation").node.attribute("Y").as_float();
		rotation.z = xmlNode.FindChildBreadth("Rotation").node.attribute("Z").as_float();

		scale.x = xmlNode.FindChildBreadth("Scale").node.attribute("X").as_float();
		scale.y = xmlNode.FindChildBreadth("Scale").node.attribute("Y").as_float();
		scale.z = xmlNode.FindChildBreadth("Scale").node.attribute("Z").as_float();
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

		// Save vertices
		memcpy(cursor, &vertices[0], verticesSize);
		cursor += verticesSize;

		// Save indices
		memcpy(cursor, &indices[0], indicesSize);
		cursor += indicesSize;

		ModuleFiles::S_Save(filePath, fileBuffer, fileSize, false);

		RELEASE(fileBuffer);

		return filePath;
	}
};