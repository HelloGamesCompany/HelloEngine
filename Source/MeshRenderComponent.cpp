#include "Headers.h"
#include "MeshRenderComponent.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModelRenderManager.h"
#include "RenderManager.h"
#include "Mesh.h"

MeshRenderComponent::MeshRenderComponent()
{
}

MeshRenderComponent::~MeshRenderComponent()
{
	RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(_modelID);
	manager->GetMap().erase(_meshID);
}

void MeshRenderComponent::InitAsDefaultCube(float3 position, float3 scale)
{
	_modelID = 0;
	RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(_modelID);

	if (!manager->initialized)
	{
		Mesh cube;
		cube.InitAsCube(position, scale);
		_meshID = manager->SetMeshInformation(cube);
	}
	else
	{

		Mesh meshInfo;
		meshInfo.InitAsMeshInformation(position, scale);
		_meshID = manager->AddMesh(meshInfo);
	}
}

void MeshRenderComponent::InitAsDefaultSphere(float3 position, float3 scale)
{
	_modelID = 1;
	RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(_modelID);


	if (!manager->initialized)
	{
		Mesh sphere;
		sphere.InitAsSphere(position, scale);
		_meshID = manager->SetMeshInformation(sphere);
	}
	else
	{

		Mesh meshInfo;
		meshInfo.InitAsMeshInformation(position, scale);
		_meshID = manager->AddMesh(meshInfo);
	}
}

void MeshRenderComponent::InitAsLoadedMesh()
{
	//TODO: determine how this funciton will be called. 
	// The body of this funciton should:
	// Acces the ModelRenderManager to check if the given mesh has already been loaded.
	// Create an instance inside the RenderManager in ModelRenderManager that manages this unique Mesh.
	// Get an ID to access that instance, so its attributes can be dynamically changed from this component.
}

Mesh& MeshRenderComponent::GetMesh()
{
	RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(_modelID);

	Mesh& meshReference = manager->GetMap()[_meshID];

	return meshReference;
}
