#pragma once
#include "Globals.h"
#include "API/API_GameObject.h"

class ComponentUIButton;

namespace API
{
	class TO_API API_UIButton
	{
	public:
		API_UIButton();
		~API_UIButton();

		API_GameObject GetGameObject();

		///Returns true the first frame is pressed
		bool OnPress();

		///Returns true every frame it is pressed.
		bool OnHold();

		bool OnHovered();

		bool getIsBlocked();

		void SetBlocked(bool);
	
	private:
		ComponentUIButton* GetComponent();
		void SetComponent(ComponentUIButton* component);
	private:
		ComponentUIButton* _UIButton = nullptr;

		friend class API_GameObject;
		friend class DragBoxUIButton;
	};
}
