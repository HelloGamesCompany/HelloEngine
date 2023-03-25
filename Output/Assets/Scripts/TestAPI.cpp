#include "TestAPI.h"
HELLO_ENGINE_API_C TestAPI* CreateTestAPI(ScriptToInspectorInterface* script)
{
	TestAPI* classInstance = new TestAPI();
	
	script->AddDragBoxMaterialComponent("Material", &classInstance->material);
	script->AddDragBoxMeshRenderer("Mesh Renderer", &classInstance->mesh);
	script->AddDragBoxMeshResource("Mesh Resource", &classInstance->otherMeshResource);
	script->AddDragBoxTextureResource("Texture Resource", &classInstance->otherTextureResource);

	return classInstance;
}

void TestAPI::Start()
{
	material.ChangeAlbedoTexture(otherTextureResource);
	mesh.ChangeMesh(otherMeshResource);
}
void TestAPI::Update()
{

}