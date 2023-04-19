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
		Engine::Console::S_Log("Trying to acces a NULLPTR UI Image");
		return *ModuleLayers::emptyAPIGameObject;
	}
	API_GameObject returnGO;
	returnGO.SetGameObject(_UIImage->GetGameObject());
	return returnGO;
}

void API::API_UIImage::FillImage(float fill)
{
	if (!_UIImage)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR UI Image. FillImage()");
		return;
	}

	if (fill > 1.0f)
		fill = 1.0f;
	if (fill < 0.0f)
		fill = 0.0f;

	_UIImage->SetFill(fill);
}

ComponentUIImage* API::API_UIImage::GetComponent()
{
	return _UIImage;
}

void API::API_UIImage::SetComponent(ComponentUIImage* component)
{
	_UIImage = component;
}

