#pragma once
#include "GameObject.h"
#include "List.h"

class Follower : public GameObject
{
private:
	GameObject* target = nullptr;

public:

	Follower(std::string name, Tag tag, Application* app, GameObject* target);

	void Start() override;

	void Update() override;

	void PostUpdate() override;

	void CleanUp() override;

	void AddFollower();

	void DestroyFollowers();

	float followerCounter = 1.0f;

	List<GameObject*> followers;

};

