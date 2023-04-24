#include "AnimationMove.h"
HELLO_ENGINE_API_C AnimationMove* CreateAnimationMove(ScriptToInspectorInterface* script)
{
	AnimationMove* classInstance = new AnimationMove();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

	script->AddDragBoxGameObject("Button Guide", &classInstance->gameObject);

	script->AddCheckBox("Move animation active", &classInstance->animationMove);

	script->AddDragFloat("Distance move X", &classInstance->distanceX);
	script->AddDragFloat("Speed move X", &classInstance->speedX);
	script->AddDragFloat("Distance move Y", &classInstance->distanceY);
	script->AddDragFloat("Speed move Y", &classInstance->speedY);

	script->AddCheckBox("Scale animation active", &classInstance->animationScale);

	script->AddDragFloat("Scale max", &classInstance->scale);
	script->AddDragFloat("Speed Scale", &classInstance->speedScale);

	script->AddCheckBox("Rotate animation active", &classInstance->animationRotate);

	script->AddDragFloat("Rotate distance", &classInstance->rotate);
	script->AddDragFloat("Speed Rotation", &classInstance->speedRotate);

	return classInstance;
}

void AnimationMove::Start()
{
	startPosX = gameObject.GetTransform().GetLocalPosition().x;
	startPosY = gameObject.GetTransform().GetLocalPosition().y;

	startScalX = gameObject.GetTransform().GetLocalScale().x;
	startScalY = gameObject.GetTransform().GetLocalScale().y;

	startRotate = gameObject.GetTransform().GetLocalRotation().z;
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

	if (animationRotate)
	{
		RotateAnimation();
	}
}

void AnimationMove::MoveAnimationX()
{
	if (startPosX - gameObject.GetTransform().GetLocalPosition().x >= distanceX)
	{
		movingRight = true;
	}
	else if (startPosX - gameObject.GetTransform().GetLocalPosition().x <= -distanceX)
	{
		movingRight = false;
	}


	if (movingRight)
	{
		gameObject.GetTransform().SetPosition(gameObject.GetTransform().GetLocalPosition().x + (0.001 * speedX), gameObject.GetTransform().GetLocalPosition().y, gameObject.GetTransform().GetLocalPosition().z);
	}
	else
	{
		gameObject.GetTransform().SetPosition(gameObject.GetTransform().GetLocalPosition().x - (0.001 * speedX), gameObject.GetTransform().GetLocalPosition().y, gameObject.GetTransform().GetLocalPosition().z);
	}
}

void AnimationMove::MoveAnimationY()
{
	if (startPosY - gameObject.GetTransform().GetLocalPosition().y >= distanceY)
	{
		movingUP = true;
	}
	else if (startPosY - gameObject.GetTransform().GetLocalPosition().y <= -distanceY)
	{
		movingUP = false;
	}


	if (movingUP)
	{
		gameObject.GetTransform().SetPosition(gameObject.GetTransform().GetLocalPosition().x, gameObject.GetTransform().GetLocalPosition().y + (0.001 * speedY), gameObject.GetTransform().GetLocalPosition().z);
	}
	else
	{
		gameObject.GetTransform().SetPosition(gameObject.GetTransform().GetLocalPosition().x, gameObject.GetTransform().GetLocalPosition().y - (0.001 * speedY), gameObject.GetTransform().GetLocalPosition().z);
	}
}

void AnimationMove::ScaleAnimation()
{
	if (startScalX - gameObject.GetTransform().GetLocalScale().x >= scale)
	{
		scaling = true;
	}
	else if (startScalX - gameObject.GetTransform().GetLocalScale().x <= -scale)
	{
		scaling = false;
	}

	if (scaling)
	{
		gameObject.GetTransform().SetScale(gameObject.GetTransform().GetLocalScale().x + (0.001 * speedScale), gameObject.GetTransform().GetLocalScale().y + (0.001 * speedScale), gameObject.GetTransform().GetLocalScale().z);
	}
	else
	{
		gameObject.GetTransform().SetScale(gameObject.GetTransform().GetLocalScale().x - (0.001 * speedScale), gameObject.GetTransform().GetLocalScale().y - (0.001 * speedScale), gameObject.GetTransform().GetLocalScale().z);
	}
}

void AnimationMove::RotateAnimation()
{
	if (startRotate - gameObject.GetTransform().GetLocalRotation().z >= rotate)
	{
		rotating = true;
	}
	else if (startRotate - gameObject.GetTransform().GetLocalRotation().z <= -rotate)
	{
		rotating = false;
	}

	if (rotating)
	{
		Console::Log("Rotating");
		gameObject.GetTransform().SetRotation(gameObject.GetTransform().GetLocalRotation().x, gameObject.GetTransform().GetLocalRotation().y, gameObject.GetTransform().GetLocalRotation().z + (0.001 * speedRotate));
	}
	else
	{
		gameObject.GetTransform().SetRotation(gameObject.GetTransform().GetLocalRotation().x, gameObject.GetTransform().GetLocalRotation().y, gameObject.GetTransform().GetLocalRotation().z - (0.001 * speedRotate));
	}
}
