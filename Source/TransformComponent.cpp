#include "Headers.h"
#include "TransformComponent.h"

TransformComponent::TransformComponent(GameObject* gameObject) : Component(gameObject)
{
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::SetPosition(float3 pos)
{
	this->position = pos;
}

void TransformComponent::SetScale(float3 s)
{
	this->scale = s;
}

void TransformComponent::SetRotation(float3 rot)
{
	this->rotation = rot;
}

void TransformComponent::Translate(float3 translation)
{
	this->position += translation;
}

void TransformComponent::Scale(float3 s)
{
	this->scale += s;
}

void TransformComponent::Rotate(float3 rotate)
{
	this->rotation += rotate;
}
