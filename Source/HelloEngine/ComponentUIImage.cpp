#include "Headers.h"
#include "ComponentUIImage.h"
#include "GameObject.h"
#include "MaterialComponent.h"

ComponentUIImage::ComponentUIImage(GameObject* gameObject) : ComponentUI(gameObject)
{

}

ComponentUIImage::~ComponentUIImage()
{
}

void ComponentUIImage::InputUpdate()
{

}

void ComponentUIImage::Serialization(json& j)
{
	json _j;

	_j["Type"] = _type;
	_j["MaterialResource"] = _material->GetResourceUID();
	_j["Enabled"] = _isEnabled;
	j["Components"].push_back(_j);
}

void ComponentUIImage::DeSerialization(json& j)
{
	_material->ChangeTexture((ResourceTexture*)ModuleResourceManager::S_LoadResource(j["MaterialResource"]));

	bool enabled = j["Enabled"];
	if (!enabled)
		Disable();

	_gameObject->transform->ForceUpdate();
}