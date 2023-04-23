#include "AnimationMove.h"
HELLO_ENGINE_API_C AnimationMove* CreateAnimationMove(ScriptToInspectorInterface* script)
{
	AnimationMove* classInstance = new AnimationMove();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

	script->AddDragBoxGameObject("Button Guide", &classInstance->gameObject);

	script->AddCheckBox("Move animation active", &classInstance->animationMove);

	script->AddDragFloat("Distance move X", &classInstance->distanceX);
	script->AddDragFloat("Distance move Y", &classInstance->distanceY);

	script->AddCheckBox("Scale animation active", &classInstance->animationScale);

	script->AddDragFloat("Scale max", &classInstance->scale);

	script->AddDragFloat("Speed move X", &classInstance->speedX);
	script->AddDragFloat("Speed move Y", &classInstance->speedY);

	return classInstance;
}

void AnimationMove::Start()
{
	startPosX = gameObject.GetTransform().GetGlobalPosition().x;
	startPosY = gameObject.GetTransform().GetGlobalPosition().y;

	startScalX = gameObject.GetTransform().GetGlobalScale().x;
	startScalY = gameObject.GetTransform().GetGlobalScale().y;
}
void AnimationMove::Update()
{
	if (animationMove)
	{
		MoveAnimationX();
		MoveAnimationY();
	}

	if (animationScale)
	{
		ScaleAnimation();
	}
}

void AnimationMove::MoveAnimationX()
{
	if (startPosX - gameObject.GetTransform().GetGlobalPosition().x >= distanceX)
	{
		movingRight = true;
	}
	else if (startPosX - gameObject.GetTransform().GetGlobalPosition().x <= -distanceX)
	{
		movingRight = false;
	}


	if (movingRight)
	{
		gameObject.GetTransform().SetPosition(gameObject.GetTransform().GetGlobalPosition().x + (0.001 * speedX), gameObject.GetTransform().GetGlobalPosition().y, gameObject.GetTransform().GetGlobalPosition().z);
	}
	else
	{
		gameObject.GetTransform().SetPosition(gameObject.GetTransform().GetGlobalPosition().x - (0.001 * speedX), gameObject.GetTransform().GetGlobalPosition().y, gameObject.GetTransform().GetGlobalPosition().z);
	}
}

void AnimationMove::MoveAnimationY()
{
	if (startPosY - gameObject.GetTransform().GetGlobalPosition().y >= distanceY)
	{
		movingUP = true;
	}
	else if (startPosY - gameObject.GetTransform().GetGlobalPosition().y <= -distanceY)
	{
		movingUP = false;
	}


	if (movingUP)
	{
		gameObject.GetTransform().SetPosition(gameObject.GetTransform().GetGlobalPosition().x, gameObject.GetTransform().GetGlobalPosition().y + (0.001 * speedY), gameObject.GetTransform().GetGlobalPosition().z);
	}
	else
	{
		gameObject.GetTransform().SetPosition(gameObject.GetTransform().GetGlobalPosition().x, gameObject.GetTransform().GetGlobalPosition().y - (0.001 * speedY), gameObject.GetTransform().GetGlobalPosition().z);
	}
}

void AnimationMove::ScaleAnimation()
{
}
