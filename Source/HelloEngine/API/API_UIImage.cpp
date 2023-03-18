#include "Headers.h"
#include "ModuleLayers.h"
#include "API_UIImage.h"
#include "ComponentUIImage.h"

API::API_UIImage::API_UIImage()
{
}

API::API_UIImage::~API_UIImage()
{
}

API::API_GameObject API::API_UIImage::GetGameObject()
{
	if (!_UIImage)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR UIButton");
		return *ModuleLayers::emptyAPIGameObject;
	}
	API_GameObject returnGO;
	returnGO.SetGameObject(_UIImage->GetGameObject());
	return returnGO;
}

float API::API_UIImage::FillImage(float _GetFillImage)
{
	if (!_UIImage)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR UI Button");
		return false;
	}

	if (_GetFillImage>1)
	{
		_GetFillImage = 1;
	}

	return _UIImage->_fillImage = _GetFillImage;
}

ComponentUIImage* API::API_UIImage::GetComponent()
{
	return _UIImage;
}

void API::API_UIImage::SetComponent(ComponentUIImage* component)
{
	_UIImage = component;
}


