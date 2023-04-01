#include "Headers.h"
#include "MaterialComponent.h"
#include "GameObject.h"
#include "ModuleResourceManager.h"

#include "SkinnedMeshRenderComponent.h"

#include "Uniform.h"

MaterialComponent::MaterialComponent(GameObject* gameObject) : Component(gameObject)
{
	_type = Type::MATERIAL;
}

MaterialComponent::~MaterialComponent()
{
	if (_resource == nullptr) return;

	_resource->Dereference();
	_resource = nullptr;
}

#ifdef STANDALONE
void MaterialComponent::OnEditor()
{
	bool created = true;

	if (_resource) _resource->material.CheckVersion();

	if (ImGui::CollapsingHeader("Material", &created, ImGuiTreeNodeFlags_DefaultOpen))
	{
		MaterialDragNDrop();
		ShaderSelectCombo();

		//INSTANCED ON A NON INSTANCED MESH ALERT MESSAGE
		if (_resource->material.GetShader()->shader.data.isIstanced &&
			GetOwnerMeshComponent()->GetMesh().isIndependent)
		{
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
				"ALERT: Using an instanced Shader/Material on a non instanced Mesh!");
		}

		//Loop Uniforms GUI
		if (_resource != nullptr && _resource->material.uniforms.size() > 0)
		{
			for (int i = 0; i < _resource->material.uniforms.size(); ++i)
			{
				if (_resource->material.uniforms[i]->data.name == "view" ||
					_resource->material.uniforms[i]->data.name == "projection" ||
					_resource->material.uniforms[i]->data.name == "model" ||
					_resource->material.uniforms[i]->data.name == "LightColor" ||
					_resource->material.uniforms[i]->data.name == "LightStrength" ||
					_resource->material.uniforms[i]->data.name == "LightPosition" ||
					_resource->material.uniforms[i]->data.name == "ViewPoint" ||
					_resource->material.uniforms[i]->data.name == "finalBonesMatrices[0]")
				{
					continue;
				}

				_resource->material.uniforms[i]->GUI();
				ImGui::Spacing();
				ImGui::Spacing();
			}

			if (ImGui::Button("Apply changes"))
			{
				_resource->Save();
			}
		}
	}
	if (!created)
		this->_gameObject->DestroyComponent(this);
}

void MaterialComponent::MarkAsDead()
{
	if (_resource == nullptr) return;

	_resourceUID = _resource->UID;
	_resource->Dereference();
	_resource = nullptr;
}

void MaterialComponent::MarkAsAlive()
{
	_resource = (ResourceMaterial*)ModuleResourceManager::S_LoadResource(_resourceUID);
	if (_resource != nullptr)
	{
		if (_resource->material.GetShader())
		{
			//Re create mesh into the RenderManager
			MeshRenderComponent* comp = _gameObject->GetComponent<MeshRenderComponent>();
			if (!comp)
			{
				comp = _gameObject->GetComponent<SkinnedMeshRenderComponent>();
			}

			if (comp) comp->CreateMesh(comp->GetResourceUID(), _resource->UID, comp->GetMeshRenderType());
		}

	}
}



void MaterialComponent::MaterialDragNDrop()
{
	std::string btnTxt = "EMPTY";

	ImGui::Button(_resource == nullptr ? btnTxt.c_str() : _resource->debugName.c_str(), { 50, 50 });

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Material"))
		{
			const uint* drop = (uint*)payload->Data;

			if (_resource != nullptr) _resource->Dereference();
			_resource = (ResourceMaterial*)ModuleResourceManager::S_LoadResource(*drop);

			if (_resource->material.GetShader())
			{
				//Re create mesh into the RenderManager
				MeshRenderComponent* comp = GetOwnerMeshComponent();

				if (comp) comp->CreateMesh(comp->GetResourceUID(), _resource->UID, comp->GetMeshRenderType());
			}
		}
	}
}

void MaterialComponent::ShaderSelectCombo()
{
	if (this->_resource == nullptr) return;
	
	std::string strSelected = "Select a shader";
	ResourceShader* resShader = _resource->material.GetShader();
	if (resShader != nullptr) strSelected = ModuleFiles::S_GetFileName(resShader->debugName);;

	std::vector<Resource*> shaderPool;

	if (ImGui::BeginCombo("##ShaderCombo", strSelected.c_str()))
	{
		shaderPool = ModuleResourceManager::S_GetResourcePool(ResourceType::SHADER);
		std::string aux;

		for (int i = 0; i < shaderPool.size(); ++i)
		{
			ResourceShader* resShader = (ResourceShader*)shaderPool[i];

			aux = ModuleFiles::S_GetFileName(resShader->debugName);

			if (resShader->shader.data.isIstanced)
				aux += "   (INSTANCED)";
			aux += "##";
			aux += shaderPool[i]->UID;

			if (ImGui::Selectable(aux.c_str()))
			{
				if (_resource->material.SetShader(shaderPool[i]->UID))
				{
					//Re create mesh into the RenderManager
					MeshRenderComponent* comp = _gameObject->GetComponent<MeshRenderComponent>();
					if (!comp)
					{
						comp = _gameObject->GetComponent<SkinnedMeshRenderComponent>();

						if (!comp) continue;
					}

					comp->CreateMesh(comp->GetResourceUID(), _resource->UID, comp->GetMeshRenderType());

				}
			}
		}

		ImGui::EndCombo();
	}
}
#endif
int MaterialComponent::GetResourceUID()
{
	if (_resource == nullptr) return -1;

	return _resource->UID;
}

MeshRenderComponent* MaterialComponent::GetOwnerMeshComponent()
{
	//Re create mesh into the RenderManager
	MeshRenderComponent* comp = _gameObject->GetComponent<MeshRenderComponent>();
	if (!comp)
	{
		comp = _gameObject->GetComponent<SkinnedMeshRenderComponent>();
	}

	return comp;
}

void MaterialComponent::Serialization(json& _j)
{
	json j;

	j["Type"] = _type;

	if (_resource != nullptr)
	{
		j["MaterialUID"] = _resource->UID;
	}
	else
	{
		j["MaterialUID"] = 0;
	}

	j["Enabled"] = _isEnabled;

	_j["Components"].push_back(j);
}

void MaterialComponent::DeSerialization(json& _j)
{
	ResourceMaterial* res = (ResourceMaterial*)ModuleResourceManager::S_LoadResource(_j["MaterialUID"]);

	if (res != nullptr)
	{
		_resource = res;
	}

	if (_resource == nullptr)
	{
		return;
	}
	

	//Load to mesh
	MeshRenderComponent* comp = GetOwnerMeshComponent();

	if (comp)
	if (_resource->material.GetShader()->shader.data.isIstanced)
		comp->CreateMesh(comp->GetResourceUID(), _resource->UID, MeshRenderType::INSTANCED);
	else
		comp->CreateMesh(comp->GetResourceUID(), _resource->UID, MeshRenderType::INDEPENDENT);

	bool enabled = _j["Enabled"];
	if (!enabled)
		Disable();
}