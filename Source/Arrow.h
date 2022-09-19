#pragma once
#include "GameObject.h"

class TaxiClient;

class Arrow : public GameObject
{
public:
	Arrow(std::string name, Tag tag, Application* app, GameObject* base, TaxiClient* target);

	void Update() override;

	void PostUpdate() override;

	void CleanUp() override;

private:
	TaxiClient* target = nullptr;
	GameObject* base = nullptr;
	vec3 dir = { 0,0,0 };

	Primitive* primitives[2] = { nullptr };

};

