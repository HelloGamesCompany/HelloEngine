#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "API/API.h"

class transformTest : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
	
	API_Transform transfromTestVariable;
	API_GameObject newGameObjectTest;
	API_MeshRenderer meshRendererTest;
	float gameObjectLife = 8.0f;
};

HELLO_ENGINE_API_C transformTest* CreatetransformTest(ScriptToInspectorInterface* script)
{
	transformTest* classInstance = new transformTest();
	script->AddDragBoxTransform("transform dragging test", &classInstance->transfromTestVariable);
	script->AddDragBoxMeshRenderer("meshTest", &classInstance->meshRendererTest);

	return classInstance;
}