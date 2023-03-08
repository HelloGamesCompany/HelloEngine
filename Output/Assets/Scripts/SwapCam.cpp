#include "SwapCam.h"
HELLO_ENGINE_API_C SwapCam* CreateSwapCam(ScriptToInspectorInterface* script)
{
	SwapCam* classInstance = new SwapCam();
	script->AddDragBoxCamera("MAIN CAMERA", &classInstance->mainCamera);
	script->AddDragBoxCamera("SECONDARY CAMERA", &classInstance->starShipCamera);
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void SwapCam::Start()
{
	mainCamera.SetAsActiveCamera();
}
void SwapCam::Update()
{
	if (Input::GetKey(KeyCode::KEY_TAB) == KeyState::KEY_DOWN)
	{

		if (mainCamActive) {
			starShipCamera.SetAsActiveCamera();
			mainCamActive = false;
		}
		else {
			mainCamera.SetAsActiveCamera();
			mainCamActive = true;
		}
	}
}