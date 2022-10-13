#include "Headers.h"
#include "MeshRenderComponent.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModelRenderManager.h"
#include "RenderManager.h"
#include "Mesh.h"

MeshRenderComponent::MeshRenderComponent(GameObject* gameObject) : Component(gameObject)
{
}

MeshRenderComponent::~MeshRenderComponent()
{
	RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(_meshID);
	manager->GetMap().erase(_instanceID);
}

void MeshRenderComponent::InitAsDefaultCube(float3 position, float3 scale)
{
	_meshID = 0;
	RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(_meshID);

	if (!manager->initialized)
	{
		Mesh cube;
		cube.InitAsCube(position, scale);
		_instanceID = manager->SetMeshInformation(cube);
	}
	else
	{

		Mesh meshInfo;
		meshInfo.InitAsMeshInformation(position, scale);
		_instanceID = manager->AddMesh(meshInfo);
	}
}

void MeshRenderComponent::InitAsDefaultSphere(float3 position, float3 scale)
{
	_meshID = 1;
	RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(_meshID);


	if (!manager->initialized)
	{
		Mesh sphere;
		sphere.InitAsSphere(position, scale);
		_instanceID = manager->SetMeshInformation(sphere);
	}
	else
	{

		Mesh meshInfo;
		meshInfo.InitAsMeshInformation(position, scale);
		_instanceID = manager->AddMesh(meshInfo);
	}
}

void MeshRenderComponent::InitAsLoadedMesh(uint meshID)
{
	_meshID = meshID + 2;

	RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(_meshID);

	Mesh instanceMesh;
	instanceMesh.InitAsMeshInformation({ 0.0f,0.0,0.0f }, {1.0f,1.0f,1.0f});

	_instanceID = manager->AddMesh(instanceMesh);

	//TODO: determine how this funciton will be called. 
	// The body of this funciton should:
	// Acces the ModelRenderManager to check if the given mesh has already been loaded.
	// Create an instance inside the RenderManager in ModelRenderManager that manages this unique Mesh.
	// Get an ID to access that instance, so its attributes can be dynamically changed from this component.
}

void MeshRenderComponent::InitAsNewMesh(std::vector<Vertex>& vertices, std::vector<uint>& indices)
{
	Mesh newMesh;
	newMesh.InitAsMesh(vertices, indices, { 1.0f,1.0f,1.0f }, { 1.0f,1.0f,1.0f });

	_meshID = Application::Instance()->renderer3D->modelRender.GetMapSize() + 2;
	RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(_meshID); // Create a renderManager.

	_instanceID = manager->SetMeshInformation(newMesh);

}

void MeshRenderComponent::OnPositionUpdate(float3 pos)
{
	GetMesh().SetPosition(pos);
}

void MeshRenderComponent::OnRotationUpdate(float3 rot)
{
	GetMesh().SetRotation(rot);
}

void MeshRenderComponent::OnScaleUpdate(float3 scale)
{
	GetMesh().SetScale(scale);
}

void MeshRenderComponent::OnTransformUpdate(float3 pos, float3 scale, float3 rot)
{
	GetMesh().SetTransform(pos, scale, rot);
}

Mesh& MeshRenderComponent::GetMesh()
{
	RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(_meshID);

	Mesh& meshReference = manager->GetMap()[_instanceID];

	return meshReference;
}
