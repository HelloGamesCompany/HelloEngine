#include "CheckRicochetTargets.h"
HELLO_ENGINE_API_C CheckRicochetTargets* CreateCheckRicochetTargets(ScriptToInspectorInterface* script)
{
    CheckRicochetTargets* classInstance = new CheckRicochetTargets();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    return classInstance;
}

void CheckRicochetTargets::Start()
{

}
void CheckRicochetTargets::Update()
{
    gameObject.GetTransform().SetPosition(atachedToGO.GetTransform().GetGlobalPosition());
    targets.clear();

    if (!atachedToGO.IsActive()) gameObject.SetActive(false);
}

void CheckRicochetTargets::OnCollisionStay(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();

    if (detectionTag == "Enemy" || detectionTag == "Boss" || detectionTag == "Thanos")
    {
        targets.push_back(other.GetGameObject());
    }
}

API_GameObject CheckRicochetTargets::GetRandomTarget()
{
    if (targets.size() > 1)
    {
        int random = rand() % targets.size();
        return targets[random];
    }
    else return gameObject;
}