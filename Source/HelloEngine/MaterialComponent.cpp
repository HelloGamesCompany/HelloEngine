#include "Headers.h"
#include "MaterialComponent.h"
#include "GameObject.h"
#include "ModuleResourceManager.h"

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

void MaterialComponent::OnEditor()
{
	bool created = true;

	if (_resource) _resource->material.CheckVersion();

	if (ImGui::CollapsingHeader("Material", &created, ImGuiTreeNodeFlags_DefaultOpen))
	{
		MaterialDragNDrop();
		ShaderSelectCombo();

		//Loop Uniforms GUI
		if (_resource != nullptr && _resource->material.uniforms.size() > 0)
		{
			for (int i = 0; i < _resource->material.uniforms.size(); ++i)
			{
				_resource->material.uniforms[i]->GUI();
				ImGui::Spacing();
				ImGui::Spacing();
			}
		}
	}
	if (!created)
		this->_gameObject->DestroyComponent(this);
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
		}
	}
}

void MaterialComponent::ShaderSelectCombo()
{
	if (this->_resource == nullptr)return;
	
	std::string strSelected = "Select a shader";
	ResourceShader* resShader = _resource->material.GetShader();
	if (resShader != nullptr) strSelected = resShader->debugName;

	std::vector<Resource*> shaderPool;

	if (ImGui::BeginCombo("##ShaderCombo", strSelected.c_str()))
	{
		shaderPool = ModuleResourceManager::S_GetResourcePool(ResourceType::SHADER);
		std::string aux;

		for (int i = 0; i < shaderPool.size(); ++i)
		{
			aux = shaderPool[i]->debugName;
			aux += "##";
			aux += shaderPool[i]->UID;
			if (ImGui::Selectable(aux.c_str()))
			{
				//Clean current Shader
				_resource->material.SetShader(shaderPool[i]->UID);
			}
		}

		ImGui::EndCombo();
	}
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

	bool enabled = _j["Enabled"];
	if (!enabled)
		Disable();
}