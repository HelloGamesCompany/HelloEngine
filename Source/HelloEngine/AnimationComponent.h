#pragma once
#include "Component.h"

struct ResourceAnimation;


class AnimationComponent : public Component
{
public:
	AnimationComponent(GameObject* gameObject);
	~AnimationComponent();

#ifdef STANDALONE
	void OnEditor() override;
#endif
	
	ResourceAnimation* resource = nullptr;
};

