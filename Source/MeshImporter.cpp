#include "Headers.h"
#include "MeshImporter.h"
#include "Mesh.h"

std::map<std::string, MeshCacheData> MeshImporter::loadedMeshes;

void MeshImporter::LoadMesh(std::string path)
{
	// Check if this file path has already been loaded.
	bool alreadyLoaded = loadedMeshes.find(path) == loadedMeshes.end();
	
	if (alreadyLoaded)
	{
		// Get only position information.

		const auto& mapItem = loadedMeshes[path];

		for (int i = 0; i < mapItem.numOfMeshes; i++)
		{
			uint meshID = mapItem.initialID + i; // Get the ID of the current Mesh

			// Create an instance of this ID inside the RenderManager of ModelRenderManager
		}
	}
	else
	{

	}

	// Load Mesh data into a Mesh class
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	ProcessNewNode(scene->mRootNode, scene);






	// Create a new RenderManager or add Mesh information as an instance
	// Create a MeshRenderComponent inside a GameObject for every Mesh, following the assimp hierachy structure.

}

void MeshImporter::ProcessNewNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessNewMesh(mesh, scene);
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNewNode(node->mChildren[i], scene);
	}
}

void MeshImporter::ProcessNewMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;	// Get Mesh information
	std::vector<uint> indices;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.position.x = mesh->mVertices->x;
		vertex.position.y = mesh->mVertices->y;
		vertex.position.z = mesh->mVertices->z;

		if (mesh->HasNormals())
		{
			vertex.normals.x = mesh->mNormals->x;
			vertex.normals.y = mesh->mNormals->y;
			vertex.normals.z = mesh->mNormals->z;
		}


		if (mesh->mTextureCoords[0])
		{
			vertex.texCoords.x = mesh->mTextureCoords[0]->x;
			vertex.texCoords.y = mesh->mTextureCoords[0]->y;

			// Tangent and bit tangent
		}
		else
		{
			vertex.texCoords = { 0.0f,0.0f };
		}

		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// Load Texture

	// Load into a Mesh object
	
	// Generate a GameObject with a RenderMeshComponent

	// Save Mesh id data inside the loadedMeshes map.
}

void MeshImporter::ProcessLoadedNode(aiNode* node, const aiScene* scene)
{
}

void MeshImporter::ProcessLoadedMesh(aiMesh* mesh, const aiScene* scene, uint meshID)
{
}

