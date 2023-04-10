#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "BossAttacks.h"
#include "BossLoop.h"
#include "API/API.h"

//#include 

class BossMovement : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
	float Rotate(API_Vector3 target, float _angle);
	void Seek(API_GameObject* seeker, API_Vector3 target, float speed);
	void Hide(API_GameObject* follower_position, API_Vector3 target_position, float speed);
	void OnCollisionEnter(API::API_RigidBody other);
	API_GameObject player;
	BossLoop* bLoop;
	API_GameObject boss;
	API_GameObject target;
	float angle = 0.0f;

	float distBP;
	float bossSpeed = 1.0f;

	bool bossDash = false;
};

