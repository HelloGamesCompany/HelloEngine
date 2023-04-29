#pragma once
#include "Globals.h"
#include "API/API_GameObject.h"

class MaterialComponent;

namespace API
{
	class TO_API API_ShaderComponent
	{
	public:
		API_ShaderComponent();
		~API_ShaderComponent();

		API_GameObject GetGameObject();

		void SetColor(float r, float g, float b, float a);
	
	private:
		MaterialComponent* GetComponent();
		void SetComponent(MaterialComponent* component);
	private:
		MaterialComponent* _materialComponent = nullptr;

		friend class API_GameObject;
		friend class DragBoxShaderComponent;
	};
}

