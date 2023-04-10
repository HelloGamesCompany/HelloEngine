#include "GoToLevelTrigger.h"
HELLO_ENGINE_API_C GoToLevelTrigger* CreateGoToLevelTrigger(ScriptToInspectorInterface* script)
{
    GoToLevelTrigger* classInstance = new GoToLevelTrigger();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddCheckBox("Is Hub Scene", &classInstance->isHub);
    script->AddInputBox("Scene path and name: ", &classInstance->scene);
    return classInstance;
}

void GoToLevelTrigger::Start()
{
    if (isHub)
    {
        if (API_QuickSave::GetBool("level1_completed")) scene = "LVL2_Blockout.HScene";
        if (API_QuickSave::GetBool("level2_completed")) scene = "LVL3_Blockout.HScene";
        if (API_QuickSave::GetBool("level3_completed")) scene = "LVL4_Blockout.HScene";
        if (API_QuickSave::GetBool("level4_completed")) scene = "LVL5_Blockout.HScene";
    }
}

void GoToLevelTrigger::Update()
{

}

void GoToLevelTrigger::OnCollisionEnter(API_RigidBody other)
{
    std::string detectionName = other.GetGameObject().GetName();
    if (detectionName == "Player")
    {
        Console::Log("trigger");
        Scene::LoadScene(scene.c_str());
    }
}
