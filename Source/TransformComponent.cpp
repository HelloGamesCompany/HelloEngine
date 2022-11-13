#include "Headers.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "ModuleCommand.h"

TransformComponent::TransformComponent(GameObject* gameObject) : Component(gameObject)
{
	_type = Type::TRANSFORM;

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
}

void TransformComponent::SetScale(float3 s)
{
	this->localTransform.scale = s;
}

void TransformComponent::SetRotation(float3 rot)
{
	this->localTransform.rotation = rot;
}

void TransformComponent::SetTransform(float3 pos, float3 scale, float3 rot)
{
	this->localTransform.position = pos;
	this->localTransform.scale = scale;
	this->localTransform.rotation = rot;
}

void TransformComponent::Translate(float3 translation)
{
	this->localTransform.position += translation;
}

void TransformComponent::Scale(float3 s)
{
	this->localTransform.scale += s;
}

void TransformComponent::Rotate(float3 rotate)
{
	this->localTransform.rotation += rotate;
}

TransformValues TransformComponent::GetGlobalTransform()
{
	// If we have no parent, we are the root gameObject. which means our local transform is also our global.
	if (_gameObject->_parent == nullptr) return localTransform; 
	
	TransformValues globalTransform;
	globalTransform.position = parentGlobalTransform.position + localTransform.position;
	globalTransform.rotation = parentGlobalTransform.rotation + localTransform.rotation;
	globalTransform.scale = { parentGlobalTransform.scale.x * localTransform.scale.x, parentGlobalTransform.scale.y * localTransform.scale.y, parentGlobalTransform.scale.z * localTransform.scale.z };
	
	return globalTransform;
}

float4x4 TransformComponent::GetGlobalMatrix()
{
	CalculateLocalMatrix();
	if (_dirtyFlag)
	{
		float4x4 parentGlobal = _gameObject->_parent != nullptr ? _gameObject->_parent->transform->GetGlobalMatrix() : float4x4::identity;
		_globalMatrix = parentGlobal * _localMatrix;
		_dirtyFlag = false;
	}
	return _globalMatrix;
}

float3 TransformComponent::GetForward()
{
	return GetGlobalMatrix().RotatePart().Col(2).Normalized();
}

float3 TransformComponent::GetRight()
{
	return GetGlobalMatrix().RotatePart().Col(0).Normalized();
}

float3 TransformComponent::GetUp()
{
	return GetGlobalMatrix().RotatePart().Col(1).Normalized();
}

void TransformComponent::OnEditor()
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat3("position", &localTransform.position[0], 0.1f))
		{
			UpdateDirtyFlag();
		}

		if(ImGui::IsItemActivated())
		{
			tempTransform = localTransform;
		}
		if(ImGui::IsItemDeactivated())
		{
			if(tempTransform != localTransform)
			{
				ModuleCommand::S_ChangeValue(&localTransform, tempTransform, localTransform, 
					std::bind(&TransformComponent::UpdateDirtyFlag, this));
			}
		}
		if (ImGui::DragFloat3("rotation", &localTransform.rotation[0], 0.1f))
		{
			UpdateDirtyFlag();
		}
		if (ImGui::IsItemActivated())
		{
			tempTransform = localTransform;
		}
		if (ImGui::IsItemDeactivated())
		{
			if (tempTransform != localTransform)
			{
				ModuleCommand::S_ChangeValue(&localTransform, tempTransform, localTransform, 
					std::bind(&TransformComponent::UpdateDirtyFlag, this));
			}
		}

		if (ImGui::DragFloat3("scale", &localTransform.scale[0], 0.1f))
		{
			UpdateDirtyFlag();
		}
		if (ImGui::IsItemActivated())
		{
			tempTransform = localTransform;
		}
		if (ImGui::IsItemDeactivated())
		{
			if (tempTransform != localTransform)
			{
				ModuleCommand::S_ChangeValue(&localTransform, tempTransform, localTransform, 
					std::bind(&TransformComponent::UpdateDirtyFlag, this));
			}
		}
	}
}

void TransformComponent::ForceUpdate()
{
	UpdateDirtyFlag();
}

void TransformComponent::UpdateDirtyFlagForChildren()
{
	std::vector<TransformComponent*> childrenTransforms;
	for (int i = 0; i < _gameObject->_children.size(); i++)
	{
		childrenTransforms.push_back((TransformComponent*)_gameObject->_children[i]->_components[0]); // We assume component Nº 0 is always Transform.
	}
	for (auto& transfrom : childrenTransforms)
	{
		transfrom->UpdateDirtyFlag();
	}
}

void TransformComponent::CalculateLocalMatrix()
{
	Quat rot = Quat::FromEulerXYZ(math::DegToRad(localTransform.rotation.x), math::DegToRad(localTransform.rotation.y), math::DegToRad(localTransform.rotation.z));
	_localMatrix = _localMatrix.FromTRS(localTransform.position, rot, localTransform.scale); // Calculate local matrix from our local transfom values.
}

void TransformComponent::UpdateDirtyFlag()
{
	// TODO: We could check if this is already true to prevent multiple calls to the function without need?
	_dirtyFlag = true;
	UpdateDirtyFlagForChildren();
}

