#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "API/API.h"

class transformTest : public HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
	
	API_Transform transfromTestVariable;
	API_GameObject newGameObjectTest;
	API_MeshRenderer meshRendererTest;
	float gameObjectLife = 8.0f;
};

