#include "Headers.h"
#include "API_Camera.h"
#include "CameraComponent.h"
#include "ModuleLayers.h"

API::API_Camera::API_Camera()
{
}

API::API_Camera::~API_Camera()
{
}

API::API_GameObject API::API_Camera::GetGameObject()
{
	if (!_camera)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR GameObject. GetGameObject()");
		return *ModuleLayers::emptyAPIGameObject;
	}
	API_GameObject returnGO;
	returnGO.SetGameObject(_camera->GetGameObject());
	return returnGO;
}

void API::API_Camera::SetAsActiveCamera()
{
	if (!_camera)
	{
		Engine::Console::S_Log("Trying to access a NULLPTR CameraComponent. SetAsActiveCamera()");;
		return;
	}
	_camera->SetAsActiveGameCamera();
}

CameraComponent* API::API_Camera::GetComponent()
{
    return nullptr;
}

void API::API_Camera::SetComponent(CameraComponent* component)
{
	_camera = component;
}
