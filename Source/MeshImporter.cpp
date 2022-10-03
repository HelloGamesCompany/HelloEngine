#include "Headers.h"
#include "MeshImporter.h"
#include "Mesh.h"


void MeshImporter::LoadMesh(std::string path)
{
	// Check if this file path has already been loaded.

	// Load Mesh data into a Mesh class
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	
	ProcessNode(scene->mRootNode, scene);


	// Create a new RenderManager or add Mesh information as an instance
	// Create a MeshRenderComponent inside a GameObject for every Mesh, following the assimp hierachy structure.

}

void MeshImporter::ProcessNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene);
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

void MeshImporter::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	// Get Mesh information
	// Load into a Mesh object
	// Generate a GameObject with a RenderMeshComponent
}
