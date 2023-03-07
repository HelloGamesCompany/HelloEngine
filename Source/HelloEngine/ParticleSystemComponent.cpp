#include "Headers.h"
#include "ParticleSystemComponent.h"
#include "ModuleRenderer3D.h"
#include "RenderManager.h"
#include "ParticleManager.h"
#include "LayerEditor.h"
#include "P_MainModule.h"




ParticleSystemComponent::ParticleSystemComponent(GameObject* gameObject) : Component(gameObject)
{
	_type = Type::PARTICLE_SYSTEM;
	_resource = nullptr;
	app = Application::Instance();
	
	ParticleEmitter.component = this;

	//Initialize Particle System Modules
	P_Module* mainModule = (P_Module*)new P_MainModule();
	mainModule->component = this;
	ParticleModules.push_back(mainModule);

	_gameObject->AddComponentOfType(Type::BILLBOARD);

	//Default Particle
	particleProps.position = _gameObject->transform->GetGlobalPosition();
	//particleProps.rot = float3::zero;
	particleProps.startsize = float3::one;
	particleProps.endsize = float3::zero;
	particleProps.speed = float3(0.0f, 1.0f, 0.0f);
	particleProps.acceleration = float3(0.0f, 0.0f, 0.0f);
	particleProps.speedVariation = float3(1.0f, 1.0f, 1.0f);
	particleProps.startColor = float4(255.0f, 255.0f, 255.0f, 1.0f); //r g b a
	particleProps.endColor = float4(255.0f, 255.0f, 255.0f, 1.0f); //r g b a

	particleProps.Lifetime = 5.0f;
	

}

ParticleSystemComponent::~ParticleSystemComponent()
{
	for (int i = 0; i < ParticleModules.size(); i++)
	{
		RELEASE(ParticleModules[i]);
	}
	ParticleModules.clear();
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

	ParticleEmitter._meshID = resourceUID;

	for (Particle& var : ParticleEmitter.ParticleList)
	{
		var._instanceID = Application::Instance()->renderer3D->renderManager.AddMesh(_resource, MeshRenderType::INSTANCED);
		//This line is needed because when you add mesh into the rendermanager it will be drawn, 
		//when we are at this point we don't want to draw the mesh of the particle till the engine is playing
		Application::Instance()->renderer3D->renderManager.GetRenderManager(resourceUID)->GetMap()[var._instanceID].draw = false;
	}

}

Mesh& ParticleSystemComponent::GetEmitterMesh()
{
	for (int i = 0; i < ParticleEmitter.ParticleList.size(); i++)
	{
		if (ParticleEmitter._meshID != -1)
		{
			Mesh& temp = ParticleEmitter.manager->GetMap()[ParticleEmitter.ParticleList[i]._instanceID];

			//Fa falta guardar la posició de cada mesh ?

			return temp;
		}
	}
	// TODO: insert return statement here
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
	{

		for (Particle& var : ParticleEmitter.ParticleList)
		{
			manager->GetMap().erase(var._instanceID);
		}
		
	}

	ParticleEmitter._meshID = -1;
}

#ifdef STANDALONE




void ParticleSystemComponent::OnEditor()
{

	bool created = true;
	if (ImGui::CollapsingHeader("Particle System", &created, ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Button("Play"))
		{
			playOnScene = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Pause"))
		{
			playOnScene = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop"))
		{
			playOnScene = false;
			if (!LayerGame::S_IsPlaying()) {
				ParticleEmitter.ResetEmitter();
			}
		}

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

					CreateEmitterMesh(*drop);

					std::string popUpmessage = "Loaded Mesh: ";
					LayerEditor::S_AddPopUpMessage(popUpmessage);

				}
				ImGui::EndDragDropTarget();
			}
			ImGui::HelpMarker("You can find .hmesh files by clicking on any model file (FBX or DAE). They will appear below the file icon in the Project window.");

			return;
		}

		for (int i = 0; i < ParticleModules.size(); i++)
		{
			ParticleModules[i]->OnEditor();
		}
	}
}

void ParticleSystemComponent::MarkAsDead()
{
	if (ParticleEmitter._meshID != -1)
	{
		DestroyEmitterMesh();

		if (_resource != nullptr)
		{
			_resource->Dereference();
			_resourceUID = _resource->UID;
			_resource = nullptr;
		}

		app->renderer3D->particleManager.RemoveEmitterInList(&ParticleEmitter);
	}
}

void ParticleSystemComponent::MarkAsAlive()
{
	CreateEmitterMesh(_resourceUID);
}

#endif // !STANDALONE

void ParticleSystemComponent::Serialization(json& j)
{
	json _j;

	_j["Type"] = _type;

	if (_resource != nullptr)
	{
		_j["ModelUID"] = _resource->modelUID;
		_j["Index inside model"] = _resource->indexInsideModel;
	}
	else
	{
		_j["ModelUID"] = 0;
		_j["Index inside model"] = 0;
	}

	if (ParticleModules.empty() == false)
	{
		_j["ParticleModules"]["ModuleMain"]["LifeTime"] = particleProps.Lifetime;
	}

	_j["Enabled"] = _isEnabled;

	j["Components"].push_back(_j);
}

void ParticleSystemComponent::DeSerialization(json& j)
{

	ResourceModel* model = (ResourceModel*)ModuleResourceManager::resources[j["ModelUID"]];

	if (model == nullptr)
	{
		Console::S_Log("A scene mesh render data was not found.");
		return;
	}

	uint index = j["Index inside model"];
	if (index < model->modelMeshes.size())
	{
		ResourceMesh* resourceMesh = model->modelMeshes[index];
		
		
		CreateEmitterMesh(resourceMesh->UID);
		
	}

	bool enabled = j["Enabled"];

}
