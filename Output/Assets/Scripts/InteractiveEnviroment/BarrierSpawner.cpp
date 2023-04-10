#include "BarrierSpawner.h"
#include "../InteractiveEnviroment/EnemyMeleeSpawner.h"
HELLO_ENGINE_API_C BarrierSpawner* CreateBarrierSpawner(ScriptToInspectorInterface* script)
{
	BarrierSpawner* classInstance = new BarrierSpawner();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

	script->AddDragBoxGameObject("Spawner 1", &classInstance->spawner1);
	script->AddDragBoxGameObject("Spawner 2", &classInstance->spawner2);
	script->AddDragBoxGameObject("Lamp 1", &classInstance->Lamp1);
	script->AddDragBoxGameObject("Lamp 2", &classInstance->Lamp2);
	script->AddDragBoxTextureResource("Lamps Texture Spawn Alive", &classInstance->materialLamps);
	script->AddDragBoxTextureResource("Lamp 1 Text Spawn Destroy", &classInstance->materialLamp1);
	script->AddDragBoxTextureResource("Lamp 2 Text Spawn Destroy", &classInstance->materialLamp2);
	script->AddDragBoxRigidBody("Barrier Collider", &classInstance->barrierRb);

	return classInstance;
}

void BarrierSpawner::Start()
{
	enemyMeleeSpawner1Ref = (EnemyMeleeSpawner*)spawner1.GetScript("EnemyMeleeSpawner");
	enemyMeleeSpawner2Ref = (EnemyMeleeSpawner*)spawner2.GetScript("EnemyMeleeSpawner");
	Lamp1.GetMaterialCompoennt().ChangeAlbedoTexture(materialLamps);
	Lamp2.GetMaterialCompoennt().ChangeAlbedoTexture(materialLamps);
}
void BarrierSpawner::Update()
{
	if (enemyMeleeSpawner1Ref->destroyed && !Lamp1TextChanged)
	{
		Lamp1.GetMaterialCompoennt().ChangeAlbedoTexture(materialLamp1);
		Lamp1TextChanged = true;
	}
	if (enemyMeleeSpawner2Ref->destroyed && !Lamp2TextChanged)
	{
		Lamp2.GetMaterialCompoennt().ChangeAlbedoTexture(materialLamp2);
		Lamp2TextChanged = true;
	}
	
	if (Lamp1TextChanged && Lamp2TextChanged && !collAsTrigger)
	{
		barrierRb.GetGameObject().GetMeshRenderer().SetActive(false);
		barrierRb.SetTrigger(true);
		collAsTrigger = true;
	}
}