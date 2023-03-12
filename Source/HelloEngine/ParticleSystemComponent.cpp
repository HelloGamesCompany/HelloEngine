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
	particleProps.acceleration = float3(1.0f, 1.0f, 1.0f);
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

	if (_resourceText != nullptr)
		_resourceText->Dereference();
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

			//Fa falta guardar la posici� de cada mesh ?

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

void ParticleSystemComponent::ChangeEmitterMeshTexture(ResourceTexture* resource)
{
	if (resource == nullptr)
	{
		ParticleEmitter._textureID = -1.0f;
		_resourceText = nullptr;

		return;
	}

	ParticleEmitter._textureID = resource->OpenGLID;

	if (_resourceText != nullptr)
		_resourceText->Dereference();

	_resourceText = resource;

	//if (resource->isTransparent && !isUI)
	//	meshRenderer->ChangeMeshRenderType(MeshRenderType::TRANSPARENCY);

}

#ifdef STANDALONE

void ParticleSystemComponent::OnEditor()
{

	bool created = true;
	if (ImGui::CollapsingHeader("Particle System", &created, ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Button("Play"))
		{
			if (ParticleEmitter.Duration > 0 || ParticleEmitter.loop)
			{
				SetPlayOnScene(true);
				SetPauseOnScene(false);
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Pause"))
		{
			if (ParticleEmitter.Duration > 0 || ParticleEmitter.loop)
			{
				SetPauseOnScene(true);
				SetPlayOnScene(false);
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop"))
		{
			if(GetPlayOnScene())
			{
				SetPlayOnScene(false);
				SetPauseOnScene(false);
				if (!LayerGame::S_IsPlaying()) {
					if (ParticleEmitter.StartDelay <= 0)
					{
						ParticleEmitter.ResetEmitter();
					}
					else
					{
						ParticleEmitter.StartDelay = ParticleEmitter.StartDelayCpy;
					}
				}
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

			if (ImGui::Button("Set Plane as Emitter Mesh"))
			{
				CreateEmitterMesh(app->renderer3D->renderManager.planeUID);
			}

			return;
		}
		else
		{
			if (ImGui::Button("Delete Emitter Mesh"))
			{
				DestroyEmitterMesh();

				std::string popUpmessage = "Mesh in the emitter Destroyed ";
				LayerEditor::S_AddPopUpMessage(popUpmessage);

			}
		}

		for (int i = 0; i < ParticleModules.size(); i++)
		{
			ParticleModules[i]->OnEditor();
		}

		if (ParticleEmitter._textureID == -1)
		{
			ImGui::TextWrapped("No texture loaded! Drag an .htext file below to load a texture ");

			ImGui::TextColored(ImVec4(1, 1, 0, 1), "Drag .htext here"); ImGui::SameLine();

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Texture"))
				{
					//Drop asset from Asset window to scene window
					const uint* drop = (uint*)payload->Data;

					ResourceTexture* resource = (ResourceTexture*)ModuleResourceManager::S_LoadResource(*drop);

					ChangeEmitterMeshTexture(resource);

					std::string popUpmessage = "Loaded Texture: ";
					LayerEditor::S_AddPopUpMessage(popUpmessage);
				}
				ImGui::EndDragDropTarget();
			}

			return;
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

	if (_resourceText != nullptr)
	{
		_resourceText->Dereference();
		_resourceTextUID = _resourceText->UID;
		_resourceText = nullptr;
	}
}

void ParticleSystemComponent::MarkAsAlive()
{
	CreateEmitterMesh(_resourceUID);

	ChangeEmitterMeshTexture((ResourceTexture*)ModuleResourceManager::S_LoadResource(_resourceTextUID));
}
#endif

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
		_j["ParticleModules"]["ModuleMain"]["BeginScale"] = { particleProps.startsize.x, particleProps.startsize.y, particleProps.startsize.z };
		_j["ParticleModules"]["ModuleMain"]["EndScale"] = { particleProps.endsize.x, particleProps.endsize.y, particleProps.endsize.z };
		_j["ParticleModules"]["ModuleMain"]["Speed"] = { particleProps.speed.x, particleProps.speed.y, particleProps.speed.z };
		_j["ParticleModules"]["ModuleMain"]["SpeedVariation"] = { particleProps.speedVariation.x, particleProps.speedVariation.y, particleProps.speedVariation.z };
		_j["ParticleModules"]["ModuleMain"]["acceleration"] = { particleProps.acceleration.x, particleProps.acceleration.y, particleProps.acceleration.z };
		_j["ParticleModules"]["ModuleMain"]["LifeTime"] = particleProps.Lifetime;
		_j["ParticleModules"]["ModuleMain"]["Duration"] = ParticleEmitter.Duration;
		_j["ParticleModules"]["ModuleMain"]["Delay"] = ParticleEmitter.StartDelay;
		_j["ParticleModules"]["ModuleMain"]["Looping"] = ParticleEmitter.loop;
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

	std::vector<float> tempstartsize = j["ParticleModules"]["ModuleMain"]["BeginScale"];
	particleProps.startsize = { tempstartsize[0],tempstartsize[1],tempstartsize[2] };
	std::vector<float> tempendsize = j["ParticleModules"]["ModuleMain"]["EndScale"];
	particleProps.endsize = { tempendsize[0],tempendsize[1],tempendsize[2] };
	std::vector<float> tempspeed = j["ParticleModules"]["ModuleMain"]["Speed"];
	particleProps.speed = { tempspeed[0],tempspeed[1],tempspeed[2] };
	std::vector<float> tempspeedVariation = j["ParticleModules"]["ModuleMain"]["SpeedVariation"];
	particleProps.speedVariation = { tempspeedVariation[0],tempspeedVariation[1],tempspeedVariation[2] };
	std::vector<float> tempacceleration = j["ParticleModules"]["ModuleMain"]["acceleration"];
	particleProps.acceleration = { tempacceleration[0],tempacceleration[1],tempacceleration[2] };
	particleProps.Lifetime = j["ParticleModules"]["ModuleMain"]["LifeTime"];
	ParticleEmitter.Duration = j["ParticleModules"]["ModuleMain"]["Duration"];
	//ParticleEmitter.StartDelay = j["ParticleModules"]["ModuleMain"]["Delay"];

	bool enabled = j["Enabled"];

}

void ParticleSystemComponent::SetPlayOnGame(bool playongame)
{
	this->playOnGame = playongame;
}

void ParticleSystemComponent::SetPlayOnScene(bool playonscene)
{
	this->playOnScene = playonscene;
}

void ParticleSystemComponent::SetPauseOnScene(bool pauseonscene)
{
	this->pauseOnScene = pauseonscene;
}
