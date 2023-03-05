#include "Headers.h"
#include "AnimationComponent.h"
#include "GameObject.h"
#include "ModuleResourceManager.h"
#include "LayerEditor.h"

AnimationComponent::AnimationComponent(GameObject* gameObject) : Component(gameObject)
{

}

AnimationComponent::~AnimationComponent()
{

}

#ifdef STANDALONE
void AnimationComponent::OnEditor()
{
	bool created = true;

	if (ImGui::CollapsingHeader("Animation Player", &created, ImGuiTreeNodeFlags_DefaultOpen))
	{
		AnimationDropArea();
	}
	if (!created)
		this->_gameObject->DestroyComponent(this);
}

void AnimationComponent::AnimationDropArea()
{
	ImGui::NewLine();

	std::string dragText = "Drag animation here";
	if (_resource != nullptr) dragText = _resource->resourcePath;

	ImGui::Button(dragText.c_str());

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Animation"))
		{
			const uint* drop = (uint*)payload->Data;

			if (_resource != nullptr) _resource->Dereference();
			//_resourceUID = _resource->UID;
			_resource = (ResourceAnimation*)ModuleResourceManager::S_LoadResource(*drop);


			std::string popUpmessage = "Loaded Animation: ";
			LayerEditor::S_AddPopUpMessage(popUpmessage);
		}
	}
}

#endif

void AnimationComponent::Serialization(json& j)
{

}

void AnimationComponent::DeSerialization(json& j)
{

}