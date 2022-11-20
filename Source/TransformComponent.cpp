#include "Headers.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "ModuleCommand.h"

TransformComponent::TransformComponent(GameObject* gameObject) : Component(gameObject)
{
	_type = Type::TRANSFORM;
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::SetPosition(float3 pos)
{
	this->localTransform.position = pos;
	UpdateDirtyFlag();
}

void TransformComponent::SetScale(float3 s)
{
	this->localTransform.scale = s;
	UpdateDirtyFlag();
}

void TransformComponent::SetRotation(float3 rot)
{
	this->localTransform.rotation = rot;
	UpdateDirtyFlag();
}

void TransformComponent::SetTransform(float3 pos, float3 scale, float3 rot)
{
	this->localTransform.position = pos;
	this->localTransform.scale = scale;
	this->localTransform.rotation = rot;
	UpdateDirtyFlag();
}

void TransformComponent::Translate(float3 translation)
{
	this->localTransform.position += translation;
	UpdateDirtyFlag();
}

void TransformComponent::Scale(float3 s)
{
	this->localTransform.scale += s;
	UpdateDirtyFlag();
}

void TransformComponent::Rotate(float3 rotate)
{
	this->localTransform.rotation += rotate;
	UpdateDirtyFlag();
}

float4x4 TransformComponent::GetGlobalMatrix(bool forceUpdate)
{
	CalculateLocalMatrix();
	if (_dirtyFlag || forceUpdate)
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

	for (auto& component : _gameObject->_components)
	{
		if (component->NeedsTransformCallback()) // Check if we need to callback our transform to some component.
		{
			component->OnTransformCallback(GetGlobalMatrix());
		}
	}

	_dirtyFlag = true;
	UpdateDirtyFlagForChildren();
}

