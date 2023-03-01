#include "Headers.h"
#include "ParticleSystemComponent.h"
#include "ModuleRenderer3D.h"
#include "RenderManager.h"
#include "ParticleManager.h"


ParticleSystemComponent::ParticleSystemComponent(GameObject* gameObject) : Component(gameObject)
{
	_type = Type::PARTICLE_SYSTEM;
	_resource = nullptr;
	app = Application::Instance();
	
	
}

ParticleSystemComponent::~ParticleSystemComponent()
{
}

void ParticleSystemComponent::CreateEmitterMesh(uint resourceUID)
{

	if (_resource != nullptr)
	{
		DestroyEmitterMesh();

		_resource->Dereference();
		_resourceUID = _resource->UID;
		_resource = nullptr;
	}

	_resource = (ResourceMesh*)ModuleResourceManager::S_LoadResource(resourceUID);

	if (_resource == nullptr)
	{
		ParticleEmitter._meshID = -1;
		return;
	}

}

void ParticleSystemComponent::OnEnable()
{
}

void ParticleSystemComponent::OnDisable()
{
}

void ParticleSystemComponent::DestroyEmitterMesh()
{
	if (ParticleEmitter._meshID == -1)
		return;


	InstanceRenderer* manager = Application::Instance()->renderer3D->renderManager.GetRenderManager(ParticleEmitter._meshID, false);
	if (manager != nullptr)
		//manager->GetMap().erase(_instanceID);

	ParticleEmitter._meshID = -1;
}

void ParticleSystemComponent::OnEditor()
{

	bool created = true;
	if (ImGui::CollapsingHeader("Particle System", &created, ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ParticleEmitter._meshID == -1)
		{
			ImGui::TextWrapped("No mesh loaded! Drag an .hmesh file below to load a mesh ");

			ImGui::TextColored(ImVec4(1, 1, 0, 1), "Drag .hmesh here"); ImGui::SameLine();

			// Create Droped mesh
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Mesh"))
				{
					const uint* drop = (uint*)payload->Data;

					//CreateMesh(*drop);

					std::string popUpmessage = "Loaded Mesh: ";
					//LayerEditor::S_AddPopUpMessage(popUpmessage);

				}
				ImGui::EndDragDropTarget();
			}
			ImGui::HelpMarker("You can find .hmesh files by clicking on any model file (FBX or DAE). They will appear below the file icon in the Project window.");


			return;
		}
	}
}

void ParticleSystemComponent::Serialization(json& j)
{
}

void ParticleSystemComponent::DeSerialization(json& j)
{
}
