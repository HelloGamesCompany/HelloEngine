#include "ArmoryBluePrints.h"
HELLO_ENGINE_API_C ArmoryBluePrints* CreateArmoryBluePrints(ScriptToInspectorInterface* script)
{
    ArmoryBluePrints* classInstance = new ArmoryBluePrints();

    script->AddDragBoxMaterialComponent("BluePrint Image Component", &classInstance->bluePrint);

    script->AddDragInt("Gun Index", &classInstance->gunIndex);

    return classInstance;
}

void ArmoryBluePrints::Start()
{
    switch (gunIndex)
    {
    case 1:
        haveBlueprint = API_QuickSave::GetBool("semiautomaticGunBlueprint");
        break;
    case 2:
        haveBlueprint = API_QuickSave::GetBool("automaticGunBlueprint");
        break;
    case 3:
        haveBlueprint = API_QuickSave::GetBool("burstGunBlueprint");
        break;
    case 4:
        haveBlueprint = API_QuickSave::GetBool("shotgunGunBlueprint");
        break;
    case 5:
        haveBlueprint = API_QuickSave::GetBool("handgunGunBlueprint");
        break;
    default:
        break;
    }

    bluePrint.GetGameObject().GetMeshRenderer().SetActive(haveBlueprint);
}
void ArmoryBluePrints::Update()
{
    
}