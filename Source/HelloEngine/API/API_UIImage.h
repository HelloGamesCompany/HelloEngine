#pragma once
#include "Globals.h"
#include "API/API_GameObject.h"

class ComponentUIImage;

namespace API
{
	class TO_API API_UIImage
	{
	public:
		API_UIImage();
		~API_UIImage();

		API_GameObject GetGameObject();
		float FillImage(float _GetFillImage);

	private:
		ComponentUIImage* GetComponent();
		void SetComponent(ComponentUIImage* component);
	private:
		ComponentUIImage* _UIImage = nullptr;

		friend class API_GameObject;
		friend class DragBoxUIImage;
	};
}
