#pragma once
#include "Globals.h"
#include "API_GameObject.h"


class ButtonState;

namespace API
{
	class TO_API API_UIButton
	{
	public:
		API_UIButton();
		~API_UIButton();

		//void GetState( ButtonState* State);
		ComponentUI* _UIButton = nullptr;
	private:
		ComponentUI* GetComponent();
		void SetComponent(ComponentUI* component);
	private:
	

	};
}
