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
	camPos.x = 0;
	camPos.y = 60.0f;
	camPos.z = -20.0f;

	camRot.x = 60.0f;
	camRot.y = 0;
	camRot.z = 0;

	gameObject.GetTransform().SetPosition(camPos);
	gameObject.GetTransform().SetRotation(camRot);

}
void CamMov::Update()
{
	desiredPosition.x = target.GetTransform().GetGlobalPosition().x + 0;
	desiredPosition.y = target.GetTransform().GetGlobalPosition().y + 60.0f;
	desiredPosition.z = target.GetTransform().GetGlobalPosition().z + -20.0f;

	smoothedPosition.x = Lerp(gameObject.GetTransform().GetGlobalPosition().x, desiredPosition.x, delay);
	smoothedPosition.y = Lerp(gameObject.GetTransform().GetGlobalPosition().y, desiredPosition.y, delay);
	smoothedPosition.z = Lerp(gameObject.GetTransform().GetGlobalPosition().z, desiredPosition.z, delay);

	gameObject.GetTransform().SetPosition(desiredPosition.x, desiredPosition.y, desiredPosition.z);
}