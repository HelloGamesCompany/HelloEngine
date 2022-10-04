#include "Headers.h"
#include "MeshImporter.h"
#include "Mesh.h"
#include "ModuleRenderer3D.h"
#include "Application.h"
#include "ModelRenderManager.h"
#include "LayerGame.h"
#include "ModuleLayers.h"

std::map<std::string, MeshCacheData> MeshImporter::loadedMeshes;
Assimp::Importer MeshImporter::importer;

void MeshImporter::LoadMesh(std::string path)
{
	// Load AiScene
	const aiScene* scene = GetAiScene(path);

	// Check if this file path has already been loaded.
	bool alreadyLoaded = loadedMeshes.find(path) != loadedMeshes.end();
	
	if (alreadyLoaded)
	{
		ProcessLoadedNode(scene->mRootNode, scene, loadedMeshes[path].initialID);
	}
	else
	{
		loadedMeshes[path].initialID = Application::Instance()->renderer3D->modelRender.GetMapSize(); // Set the ID of the first mesh inside this Model
		loadedMeshes[path].numOfMeshes = 0;
		ProcessNewNode(scene->mRootNode, scene, path);
	}


	// Create a new RenderManager or add Mesh information as an instance
	// Create a MeshRenderComponent inside a GameObject for every Mesh, following the assimp hierachy structure.

}

const aiScene* MeshImporter::GetAiScene(std::string path)
{
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
	}
	return scene;
}

void MeshImporter::ProcessNewNode(aiNode* node, const aiScene* scene, std::string path)
{
	// Create empty Gameobject 
	loadedMeshes[path].numOfMeshes += node->mNumMeshes; // Increase the number of meshes for every mesh inside this node.
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// Process mesh and create a GameObject with a MeshRenderComponent that is child to the epmty game object
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessNewMesh(mesh, scene);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		// Creates an empty Gameobject that is children to the empty gameObject created here.
		ProcessNewNode(node->mChildren[i], scene, path);
	}
}

void MeshImporter::ProcessNewMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;	
	std::vector<uint> indices;

	// Get Mesh information
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

	// TODO: Load texture data 

	// Load into a Mesh object
	MeshRenderComponent* newMeshComponent = new MeshRenderComponent();

	newMeshComponent->InitAsNewMesh(vertices, indices);

	// TODO: Generate a GameObject with a RenderMeshComponent
	LayerGame* game = (LayerGame*)Application::Instance()->layers->layers[LayersID::GAME];//TEMPORAL CODE
	game->meshComponentTest.push_back(newMeshComponent);
}

void MeshImporter::ProcessLoadedNode(aiNode* node, const aiScene* scene, uint firstMeshID)
{
	// Create an empty GameObject that represents the Node
	ModelRenderManager* renderManager = &Application::Instance()->renderer3D->modelRender;
	uint meshID = firstMeshID;

	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// Create a GameObject with a MeshRenderComponent that represents the Mesh
		MeshRenderComponent newMeshRender;
		newMeshRender.InitAsLoadedMesh(meshID++);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		// Creates an empty Gameobject that is children to the empty gameObject created here.
		ProcessLoadedNode(node->mChildren[i], scene, meshID);
	}
}

