#pragma once
#include "GameObject.h"
class Booster : public GameObject
{
public:

	Booster(std::string name, Tag tag, Application* app, vec3 pos, vec3 dimensions, bool boost);

	bool boost = true;


};

