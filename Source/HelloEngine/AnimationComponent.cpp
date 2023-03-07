#include "Headers.h"
#include "AnimationComponent.h"
#include "GameObject.h"
#include "ModuleResourceManager.h"
#include "LayerEditor.h"
#include "SkinnedMeshRenderComponent.h"

AnimationComponent::AnimationComponent(GameObject* gameObject) : Component(gameObject)
{
	isPlaying = false;
	isLoop = false;
	isStayLast = false;

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

		if (_resource != nullptr)
		{
			if (isPlaying)
			{
				if (ImGui::Button("Stop"))
				{
					StopAnimation();
				}
			}
			else
			{
				if (ImGui::Button("Play"))
				{
					PlayAnimation();
				}
			}
			

			if (isPlaying)
			{
				ImGui::Text(std::to_string(currentKeyframe).c_str());
				ImGui::Text(std::to_string(animStartPlayTime).c_str());
				ImGui::Text(std::to_string(animEndPlayTime).c_str());
				ImGui::Text(std::to_string(currentTime).c_str());
			}
		}

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

void AnimationComponent::PlayAnimation()
{
	if (_resource == nullptr) return;

	isPlaying = true;
	LayerGame::S_AddAnimationComponent(this);

	animStartPlayTime = EngineTime::EngineTimeDeltaTime() * 1000;
	animEndPlayTime = animStartPlayTime + (_resource->animation.durationTicks * _resource->animation.bones[0].keyframes.size());
	currentTime = animStartPlayTime;

}

void AnimationComponent::StopAnimation()
{
	isPlaying = false;
	LayerGame::S_RemoveAnimationComponent(this);

	animStartPlayTime = 0;
	animEndPlayTime = 0;
	currentTime = 0;
}

void AnimationComponent::UpdateAnimation()
{
	if (_resource == nullptr) return;
	if (!isPlaying) return;

	SkinnedMeshRenderComponent* skin = _gameObject->GetComponent<SkinnedMeshRenderComponent>();

	if (skin != nullptr && skin->HasBones())
	{
		skin->UpdateBones();

		Animation3D& animation = _resource->animation;
		float engineTime = EngineTime::EngineTimeDeltaTime();



		//float animationTime = CalculateScaleFactor()

		for (int i = 0; i < skin->goBonesArr.size(); ++i)
		{

			//float animationTime = CalculateScaleFactor()
			//InterpolateMatrix(animation.bones[i].keyframes, animation.bones[i].keyframes, engineTime);
		}

		currentTime += engineTime;

		//if(currentTime >= animEndPlayTime)

	}
}


float AnimationComponent::CalculateScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
{
	float scaleFactor = 0.0f;
	float midWayLength = animationTime - lastTimeStamp;
	float framesDiff = nextTimeStamp - lastTimeStamp;

	scaleFactor = midWayLength / framesDiff;

	return scaleFactor;
}

float3x4 AnimationComponent::InterpolateMatrix(float3x4 currentMatrix, float3x4 nextMatrix, float animationTime)
{
	float3x4 toReturn;
	
	//Current
	float3 currentTranslate = currentMatrix.TranslatePart();
	float3 currentScale = currentMatrix.GetScale();
	Quat currentRot = math::Quat(currentMatrix.RotatePart());
	
	//Next
	float3 nextTranslate = nextMatrix.TranslatePart();
	float3 nextScale = nextMatrix.GetScale();
	Quat nextRot = math::Quat(currentMatrix.RotatePart());

	//Translate interpolation
	float3 finalTranslate = math::Lerp(currentTranslate, nextTranslate, animationTime);

	//Scale interpolation
	float3 finalScale = math::Lerp(currentScale, nextScale, animationTime);

	//Quat interpolation
	Quat finalRot = math::Slerp(currentRot, nextRot, animationTime);

	toReturn.FromTRS(finalTranslate, finalRot, finalScale);

	return toReturn;
}

void AnimationComponent::Serialization(json& j)
{

}

void AnimationComponent::DeSerialization(json& j)
{

}