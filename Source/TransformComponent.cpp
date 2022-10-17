#include "Headers.h"
#include "TransformComponent.h"
#include "GameObject.h"

TransformComponent::TransformComponent(GameObject* gameObject) : Component(gameObject)
{
	// If this is not the root node, we need the parents global transform.
	if (this->_gameObject->_parent)
	{
		TransformComponent* parentTransform = nullptr;
		parentTransform = _gameObject->_parent->GetComponent<TransformComponent>();
		parentGlobalTransform = parentTransform->GetGlobalTransform();
	}
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::SetPosition(float3 pos)
{
	this->localTransform.position = pos;
	UpdatePosition();
}

void TransformComponent::SetScale(float3 s)
{
	this->localTransform.scale = s;
	UpdateScale();
}

void TransformComponent::SetRotation(float3 rot)
{
	this->localTransform.rotation = rot;
	UpdateRotation();
}

void TransformComponent::SetTransform(float3 pos, float3 scale, float3 rot)
{
	this->localTransform.position = pos;
	this->localTransform.scale = scale;
	this->localTransform.rotation = rot;
	UpdateTransform();
}

void TransformComponent::Translate(float3 translation)
{
	this->localTransform.position += translation;
	UpdatePosition();
}

void TransformComponent::Scale(float3 s)
{
	this->localTransform.scale += s;
	UpdateScale();
}

void TransformComponent::Rotate(float3 rotate)
{
	this->localTransform.rotation += rotate;
	UpdateRotation();
}

void TransformComponent::OnPositionUpdate(float3 pos)
{
	parentGlobalTransform.position = pos;
	UpdatePosition();
}

void TransformComponent::OnRotationUpdate(float3 rot)
{
	parentGlobalTransform.rotation = rot;
	UpdateRotation();
}

void TransformComponent::OnScaleUpdate(float3 scale)
{
	parentGlobalTransform.scale = scale;
	UpdateScale();
}

void TransformComponent::OnTransformUpdate(float3 pos, float3 scale, float3 rot)
{
	parentGlobalTransform.position = pos;
	parentGlobalTransform.rotation = rot;
	parentGlobalTransform.scale = scale;
	UpdateTransform();
}

TransformValues TransformComponent::GetGlobalTransform()
{
	// If we have no parent, we are the root gameObject. which means our local transform is also our global.
	if (_gameObject->_parent == nullptr) return localTransform; 
	
	TransformValues globalTransform;
	globalTransform.position = parentGlobalTransform.position + localTransform.position;
	globalTransform.rotation = parentGlobalTransform.rotation + localTransform.rotation;
	globalTransform.scale = parentGlobalTransform.scale + localTransform.scale;
	
	return globalTransform;
}

void TransformComponent::UpdatePosition()
{
	float3 globalPosition = parentGlobalTransform.position + localTransform.position;
	// Give current position change to the transform component of every child of this transform's gameobject.
	for (int i = 0; i < _gameObject->_children.size(); i++)
	{
		_gameObject->_children[i]->_components[0]->OnPositionUpdate(globalPosition);
	}

	// Give current position change to every component linked to this transform's gameobject.
	for (int i = 1; i < _gameObject->_components.size(); i++)
	{
		_gameObject->_components[i]->OnPositionUpdate(globalPosition);
	}
}

void TransformComponent::UpdateRotation()
{
	float3 totalRotation = parentGlobalTransform.rotation + localTransform.rotation;
	// Give current position change to the transform component of every child of this transform's gameobject.
	for (int i = 0; i < _gameObject->_children.size(); i++)
	{
		_gameObject->_children[i]->_components[0]->OnRotationUpdate(totalRotation);
	}

	// Give current position change to every component linked to this transform's gameobject.
	for (int i = 1; i < _gameObject->_components.size(); i++)
	{
		_gameObject->_components[i]->OnRotationUpdate(totalRotation);
	}
}

void TransformComponent::UpdateScale()
{
	float3 totalScale = parentGlobalTransform.scale + localTransform.scale;
	// Give current position change to the transform component of every child of this transform's gameobject.
	for (int i = 0; i < _gameObject->_children.size(); i++)
	{
		_gameObject->_children[i]->_components[0]->OnScaleUpdate(totalScale);
	}

	// Give current position change to every component linked to this transform's gameobject.
	for (int i = 1; i < _gameObject->_components.size(); i++)
	{
		_gameObject->_components[i]->OnScaleUpdate(totalScale);
	}
}

void TransformComponent::UpdateTransform()
{
	TransformValues globalTransform = GetGlobalTransform();
	// Give current position change to the transform component of every child of this transform's gameobject.
	for (int i = 0; i < _gameObject->_children.size(); i++)
	{
		_gameObject->_children[i]->_components[0]->OnTransformUpdate(globalTransform.position, globalTransform.scale, globalTransform.rotation);
	}

	// Give current position change to every component linked to this transform's gameobject. i = 1 so we skip this transform instance
	for (int i = 1; i < _gameObject->_components.size(); i++)
	{
		_gameObject->_components[i]->OnTransformUpdate(globalTransform.position, globalTransform.scale, globalTransform.rotation);
	}
}