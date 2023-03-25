#pragma once
#include "Globals.h"
#include "API_GameObject.h"

class MaterialComponent;
class DragBoxMaterial;

namespace API
{
	class TO_API API_Material
	{
	public:
		API_Material();
		~API_Material();

		API_GameObject GetGameObject();

		void ChangeAlbedoTexture(uint textureUID);

	private:
		MaterialComponent* GetComponent();
		void SetComponent(MaterialComponent* component);
	private:
		MaterialComponent* _material = nullptr;

		friend class API_GameObject;
		friend class DragBoxMaterialComponent;
	};
}

