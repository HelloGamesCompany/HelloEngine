
#pragma once
#include "Globals.h"
#include "API/API_GameObject.h"

class TextRendererComponent;

namespace API
{
	class TO_API API_UIText
	{
	public:
		API_UIText();
		~API_UIText();

		API_GameObject GetGameObject();

		//void SetEnable(bool);
		void SetText(const char* text);

	private:
		TextRendererComponent* GetComponent();
		void SetComponent(TextRendererComponent* component);
	private:
		TextRendererComponent* _UIText = nullptr;

		friend class API_GameObject;
		friend class DragBoxUIText;
	};
}