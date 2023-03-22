#include "ButtonActionsTest.h"
HELLO_ENGINE_API_C ButtonActionsTest* CreateButtonActionsTest(ScriptToInspectorInterface* script)
{
	ButtonActionsTest* classInstance = new ButtonActionsTest();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxUIButton("button", &classInstance->apiButton);
	script->AddDragFloat("minY", &classInstance->minY);
	script->AddDragFloat("maxY", &classInstance->maxY);
	script->AddDragFloat("Vel", &classInstance->animVel);
	//script->AddDragBoxUIImage("image", &classInstance->apiImage);

	return classInstance;
}

void ButtonActionsTest::Start()
{
	isHovering = false;
	Console::Log("Button started!");
	//gameObject.GetTransform().SetScale(API_Vector3(1.f, 1.f, 1.f));
	defaultPos = gameObject.GetTransform().GetGlobalPosition();

}
void ButtonActionsTest::Update()
{
	if (apiButton.OnHovered()==true)
	{
		if (isHovering == false) 
		{ 
			isHovering = true; 
			isGoingUp = true;
			
			//API_UIImage image = apiImage;
			//image.GetGameObject().SetActive(true);
			//go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
		}
		//Console::Log("im pressed");
		
		//gameObject.GetTransform().SetPosition(defaultPos+API_Vector3(0,0.5f,0));
	}
	else 
	{
		if (isHovering == true) 
		{ 
			isHovering = false; 
			gameObject.GetTransform().SetPosition(defaultPos);
			//API_UIImage image = apiImage;
			//image.GetGameObject().SetActive(false);
		}
	
	}

	if (isHovering == true)
	{
		if (isGoingUp == true) {
			gameObject.GetTransform().Translate(0, animVel, 0);
			if (gameObject.GetTransform().GetGlobalPosition().y > defaultPos.y + maxY)
			{
				isGoingUp = false;
			}
		}
		else {
			gameObject.GetTransform().Translate(0, -animVel, 0);
			if (gameObject.GetTransform().GetGlobalPosition().y < defaultPos.y + minY)
			{
				isGoingUp = true;
			}
		}
	}
	
}