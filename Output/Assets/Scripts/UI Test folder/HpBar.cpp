#include "HpBar.h"
HELLO_ENGINE_API_C HpBar* CreateHpBar(ScriptToInspectorInterface* script)
{
	HpBar* classInstance = new HpBar();
	script->AddDragFloat("HP", &classInstance->hp);
	script->AddDragFloat("MAX HP", &classInstance->maxHp);

	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void HpBar::Start()
{
	FirstPosX = gameObject.GetTransform().GetLocalPosition().x;
	FirstPosY = gameObject.GetTransform().GetLocalPosition().y;
	FirstPosZ = gameObject.GetTransform().GetLocalPosition().z;
}
void HpBar::Update()
{
	float finalScale = hp * 0.4f / maxHp;

	if (finalScale != gameObject.GetTransform().GetLocalScale().x) {

		float differenceHp = maxHp - hp;

		float offsetX = differenceHp * 0.0036f;
		//float offsetY = differenceHp * 0.0014f;

		//gameObject.GetTransform().SetPosition(FirstPosX - offsetX, FirstPosY - offsetY, gameObject.GetTransform().GetLocalPosition().z);
		gameObject.GetTransform().SetPosition(FirstPosX - offsetX, FirstPosY, gameObject.GetTransform().GetLocalPosition().z);
	}


	gameObject.GetTransform().SetScale(finalScale, 0.12f, 0.5f);


}