#include "Headers.h"
#include "ModuleLayers.h"
#include "API_UIInput.h"
#include "ComponentUIInput.h"

API::API_UIInput::API_UIInput()
{
}

API::API_UIInput::~API_UIInput()
{
}

API::API_GameObject API::API_UIInput::GetGameObject()
{
	if (!_UIInput)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR UIInput");
		return *ModuleLayers::emptyAPIGameObject;
	}
	API_GameObject returnGO;
	returnGO.SetGameObject(_UIInput->GetGameObject());
	return returnGO;
}


ComponentUIInput* API::API_UIInput::GetComponent()
{
	return _UIInput;
}

void API::API_UIInput::SetComponent(ComponentUIInput* component)
{
	_UIInput = component;
}

void API::API_UIInput::SetEnable(bool isActive)
{
	if (!_UIInput)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR UIInput. SetEnable()");
		return;
	}
	_UIInput->_isComponentEnable = isActive;
}

bool API::API_UIInput::IsEnabled()
{
	if (!_UIInput)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR UIInput. IsEnabled()");
		return false;
	}
	return _UIInput->IsEnabled();
}

