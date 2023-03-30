#pragma once
#include "Globals.h"
#include "API/API_GameObject.h"

class ComponentUIInput;

namespace API
{
	class TO_API API_UIInput
	{
	public:
		API_UIInput();
		~API_UIInput();

		API_GameObject GetGameObject();

	private:
		ComponentUIInput* GetComponent();
		void SetComponent(ComponentUIInput* component);
	private:
		ComponentUIInput* _UIInput = nullptr;

		friend class API_GameObject;
		friend class DragBoxUIInput;
	};
}