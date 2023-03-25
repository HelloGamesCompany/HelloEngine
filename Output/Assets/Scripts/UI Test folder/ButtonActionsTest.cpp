#include "ButtonActionsTest.h"
HELLO_ENGINE_API_C ButtonActionsTest* CreateButtonActionsTest(ScriptToInspectorInterface* script)
{
	ButtonActionsTest* classInstance = new ButtonActionsTest();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxUIButton("Button", &classInstance->apiButton);
	script->AddDragFloat("minY", &classInstance->minY);
	script->AddDragFloat("maxY", &classInstance->maxY);
	script->AddDragFloat("Vel", &classInstance->animVel);

	return classInstance;
}

void ButtonActionsTest::Start()
{

	isHovering = false;

	defaultPos = gameObject.GetTransform().GetGlobalPosition();

	hasStarted = false;

}
void ButtonActionsTest::Update()
{
	if (hasStarted == false)
	{
	//{
		//if ((apiButton.OnHovered() == false ) && (apiButton.OnPress() == false)) {

		//	isHovering = false;

			defaultPos = gameObject.GetTransform().GetGlobalPosition();
		//	//defaultPos = API_Vector3(0,0,-0.003f);
			hasStarted = true;
		//}
		
	}
	else {
		if (apiButton.OnHovered() == true)
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
			gameObject.GetTransform().SetPosition(defaultPos);
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

		if (apiButton.OnPress() == true)
		{
			hasStarted = false;
		}
	}

	
	
}