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

    if (!atachedToGO.IsActive()) gameObject.SetActive(false);
}

void CheckRicochetTargets::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();

    targets.clear();

    if (detectionTag == "Enemy")
    {
        targets.push_back(other.GetGameObject());
    }
}

API_GameObject CheckRicochetTargets::GetRandomTarget()
{
    int random = rand() % targets.size();
    if (targets.size() > 0) return targets[random];
    else return gameObject;
}