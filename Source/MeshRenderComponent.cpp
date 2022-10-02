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
}

void MeshRenderComponent::InitAsDefaultCube()
{
	modelID = 0;
	RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(modelID);

	Mesh cube;
	cube.InitAsCube({ 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f });

	if (!manager->initialized) meshID = manager->SetMeshInformation(cube);
	else meshID = manager->AddMesh(cube);

}

void MeshRenderComponent::InitAsDefaultSphere()
{
	modelID = 1;
	RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(modelID);

	Mesh cube;
	cube.InitAsSphere({ 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f });

	if (!manager->initialized) meshID = manager->SetMeshInformation(cube);
	else meshID = manager->AddMesh(cube);
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
	RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(modelID);

	Mesh& meshReference = manager->GetMap()[meshID];

	return meshReference;
	// TODO: Insertar una instrucción "return" aquí
}
