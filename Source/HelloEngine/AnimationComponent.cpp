#include "Headers.h"
#include "AnimationComponent.h"


AnimationComponent::AnimationComponent(GameObject* gameObject) : Component(gameObject)
{

}

AnimationComponent::~AnimationComponent()
{

}

#ifdef STANDALONE
void AnimationComponent::OnEditor()
{

}

#endif