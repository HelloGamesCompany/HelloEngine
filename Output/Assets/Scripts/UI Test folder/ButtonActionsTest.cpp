#include "ButtonActionsTest.h"
HELLO_ENGINE_API_C ButtonActionsTest* CreateButtonActionsTest(ScriptToInspectorInterface* script)
{
	ButtonActionsTest* classInstance = new ButtonActionsTest();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragFloat("minY", &classInstance->minY);
	script->AddDragFloat("maxY", &classInstance->maxY);
	script->AddDragFloat("Vel", &classInstance->animVel);

	return classInstance;
}

void ButtonActionsTest::Start()
{
	isHovering = false;

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
		}

	}
	else 
	{
		if (isHovering == true) 
		{ 
			isHovering = false; 
			gameObject.GetTransform().SetPosition(defaultPos);
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
	else {
		gameObject.GetTransform().SetPosition(defaultPos);
	}
	
}