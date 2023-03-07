#pragma once
#include "Globals.h"
#include "API_GameObject.h"

class CameraComponent;

namespace API
{
	class TO_API API_Camera
	{
	public:
		API_Camera();
		~API_Camera();

		API_GameObject GetGameObject();

		void SetAsActiveCamera();

	private:
		CameraComponent* GetComponent();
		void SetComponent(CameraComponent* component);
	private:
		CameraComponent* _camera = nullptr;

		friend class API_GameObject;
		friend class DragBoxCamera;
	};
}

