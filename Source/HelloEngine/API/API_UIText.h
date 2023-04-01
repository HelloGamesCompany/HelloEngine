
#pragma once
#include "Globals.h"
#include "API/API_GameObject.h"

class ComponentUIText;

namespace API
{
	class TO_API API_UIText
	{
	public:
		API_UIText();
		~API_UIText();

		API_GameObject GetGameObject();

		//void SetEnable(bool);

	private:
		ComponentUIText* GetComponent();
		void SetComponent(ComponentUIText* component);
	private:
		ComponentUIText* _UIText = nullptr;

		friend class API_GameObject;
		friend class DragBoxUIText;
	};
}