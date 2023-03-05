#pragma once
#include "Component.h"

struct ResourceAnimation;


class AnimationComponent : public Component
{
public:
	AnimationComponent(GameObject* gameObject);
	~AnimationComponent();

	void Serialization(json& j) override;
	void DeSerialization(json& j) override;
#ifdef STANDALONE
	void OnEditor() override;

private:
	void AnimationDropArea();

#endif
	
public:

	ResourceAnimation* _resource = nullptr;
};

