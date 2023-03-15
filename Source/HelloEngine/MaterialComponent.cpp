#include "Headers.h"
#include "MaterialComponent.h"
#include "GameObject.h"
#include "ModuleResourceManager.h"

MaterialComponent::MaterialComponent(GameObject* gameObject) : Component(gameObject)
{

}

MaterialComponent::~MaterialComponent()
{

}

void MaterialComponent::Serialization(json& j)
{

}

void MaterialComponent::DeSerialization(json& j)
{

}

void MaterialComponent::OnEditor()
{
	bool created = true;

	if (ImGui::CollapsingHeader("Material", &created, ImGuiTreeNodeFlags_DefaultOpen))
	{
		MaterialDragNDrop();
		ShaderSelectCombo();
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
				////Clean current Shader
				//if (material->GetShader() != nullptr)
				//{
				//	if (!material->GetShader()->uuid.empty())
				//	{
				//		ResourceShader* ress = (ResourceShader*)resInstance->resources.at(material->GetShader()->uuid);
				//		if (ress != nullptr) ress->RemoveShaderFromMat(material);
				//		//if (ress->shader == nullptr) LOG(LOG_TYPE::ATTENTION, "RC 0: Unloading shader '%s' from memory!", ress->GetLibraryFile().c_str());
				//	}
				//}

				////Set new shader
				///*material->SetShader(ShaderManager::ImportFromLibrary(shaderPool[i]));
				//shaderPool[i]->IncreaseRC();*/

				//shaderPool[i]->SetShaderToMat(material);
			}
		}

		ImGui::EndCombo();
	}
}