#pragma once
#include "Globals.h"
#include "API/API_GameObject.h"

class TO_API HelloBehavior
{
public:
	HelloBehavior() {};
	virtual ~HelloBehavior() {};

	virtual void Init() {};
	virtual void Start() {};
	virtual void Update() {};
public:
	API::API_GameObject gameObject;

	friend class LayerGame;
};

