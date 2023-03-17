#include "CamMov.h"
HELLO_ENGINE_API_C CamMov* CreateCamMov(ScriptToInspectorInterface* script)
{
	CamMov* classInstance = new CamMov();
	script->AddDragBoxGameObject("Target", &classInstance->target);
	script->AddDragFloat("Delay", &classInstance->delay);
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

float CamMov::Lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

void CamMov::Start()
{

	camPos.x = gameObject.GetTransform().GetGlobalPosition().x;
	camPos.y = gameObject.GetTransform().GetGlobalPosition().y + 7.931;
	camPos.z = gameObject.GetTransform().GetGlobalPosition().z-12.462;

	camRot.x = gameObject.GetTransform().GetGlobalRotation().x + 40.194f;
	camRot.y = gameObject.GetTransform().GetGlobalRotation().y;
	camRot.z = gameObject.GetTransform().GetGlobalRotation().z;

	gameObject.GetTransform().SetPosition(camPos);
	gameObject.GetTransform().SetRotation(camRot);

}

void CamMov::Update()
{

	if (target.GetTransform().GetGlobalPosition().x > gameObject.GetTransform().GetGlobalPosition().x + 7
		|| target.GetTransform().GetGlobalPosition().x < gameObject.GetTransform().GetGlobalPosition().x - 7
		|| target.GetTransform().GetGlobalPosition().z > gameObject.GetTransform().GetGlobalPosition().z + 4 + 13
		|| target.GetTransform().GetGlobalPosition().z < gameObject.GetTransform().GetGlobalPosition().z + 7) {
		delay += 0.001;
	}
	else {
		delay = 0.02;
	}

	desiredPosition.x = target.GetTransform().GetGlobalPosition().x + camPos.x;
	desiredPosition.y = target.GetTransform().GetGlobalPosition().y + camPos.y;
	desiredPosition.z = target.GetTransform().GetGlobalPosition().z + camPos.z;

	smoothedPosition.x = Lerp(gameObject.GetTransform().GetGlobalPosition().x, desiredPosition.x, delay);
	smoothedPosition.y = Lerp(gameObject.GetTransform().GetGlobalPosition().y, desiredPosition.y, delay);
	smoothedPosition.z = Lerp(gameObject.GetTransform().GetGlobalPosition().z, desiredPosition.z, delay);

	gameObject.GetTransform().SetPosition(smoothedPosition.x, smoothedPosition.y, smoothedPosition.z);
}