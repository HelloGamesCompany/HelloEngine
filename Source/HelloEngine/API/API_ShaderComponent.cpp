#include "Headers.h"
#include "ModuleLayers.h"
#include "API_ShaderComponent.h"
#include "MaterialComponent.h"
#include "ModuleResourceManager.h"
#include "Uniform.h"

API::API_ShaderComponent::API_ShaderComponent()
{
}

API::API_ShaderComponent::~API_ShaderComponent()
{
}

API::API_GameObject API::API_ShaderComponent::GetGameObject()
{
	if (!_materialComponent)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR API_ShaderComponent");
		return *ModuleLayers::emptyAPIGameObject;
	}
	API_GameObject returnGO;
	returnGO.SetGameObject(_materialComponent->GetGameObject());
	return returnGO;
}

void API::API_ShaderComponent::SetColor(float r, float g, float b, float a)
{
	if (!_materialComponent)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR API_ShaderComponent");
		return;
	}
	std::vector<Uniform*>& uniforms = _materialComponent->_resource->material.uniforms;
	for (int i = 0; i < uniforms.size(); ++i)
	{
		if (uniforms[i]->data.name == "ColourTest")
		{
			float* data = (float*)uniforms[i]->data.value;
			*data = r;
			++data;
			*data = g;
			++data;
			*data = b;
			++data;
			*data = a;
			break;
		}
	}
}

MaterialComponent* API::API_ShaderComponent::GetComponent()
{
    return _materialComponent;
}

void API::API_ShaderComponent::SetComponent(MaterialComponent* component)
{
	_materialComponent = component;
}
