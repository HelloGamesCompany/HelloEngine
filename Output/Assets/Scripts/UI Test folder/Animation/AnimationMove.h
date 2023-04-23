#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class AnimationMove : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void MoveAnimationX();
	void MoveAnimationY();
	void ScaleAnimation();

	API_GameObject gameObject;

	bool animationMove = false;
	bool animationScale = false;

	float distanceX = 0;
	float distanceY = 0;
	float scale = 0;

	float speedX= 0;
	float speedY= 0;

	float startPosX;
	float startPosY;

	float startScalX;
	float startScalY;

	//true is up or right, false is oposite
	bool movingUP = true;
	bool movingRight = true;
};

