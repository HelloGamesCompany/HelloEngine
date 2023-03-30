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


