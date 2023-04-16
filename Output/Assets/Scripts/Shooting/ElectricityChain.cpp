#include "ElectricityChain.h"
#include "ProjectilePull.h"
#include "../Enemies/Enemy.h"
HELLO_ENGINE_API_C ElectricityChain* CreateElectricityChain(ScriptToInspectorInterface* script)
{
    ElectricityChain* classInstance = new ElectricityChain();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    return classInstance;
}

void ElectricityChain::Start()
{

}

void ElectricityChain::Update()
{
    if (destroy)
    {
        pull->electricityChainExeptionsAmountActive[exeptionsVectorIndex]--;
        Enemy* enemy = (Enemy*)atachedToGO.GetScript("Enemy");
        if (enemy) enemy->TakeDamage(damage, resistanceDamage);
        gameObject.SetActive(false);
    }

    delay -= Time::GetDeltaTime();
    if (delay <= 0.0f) triggerActive = true;

    gameObject.GetTransform().SetPosition(atachedToGO.GetTransform().GetGlobalPosition());
}

void ElectricityChain::OnCollisionEnter(API::API_RigidBody other)
{
    if (!triggerActive && chainCount < ELECTRICITY_CHAIN_COUNT) return;

    std::string detectionTag = other.GetGameObject().GetTag();

    if (detectionTag == "Enemy")
    {
        std::vector<uint> exeptions = pull->electricityChainExeptions[exeptionsVectorIndex];
        if (std::find(exeptions.begin(), exeptions.end(), other.GetGameObject().GetUID()) == exeptions.end()) // if current enemy is not on vector
        {
            pull->electricityChainExeptions[exeptionsVectorIndex].push_back(other.GetGameObject().GetUID());
            pull->electricityChainExeptionsAmountActive[exeptionsVectorIndex]++;
            pull->LauchELECTRICITY_CHAIN(ELECTRICITY_DELAY, damage, resistanceDamage, other.GetGameObject(), exeptionsVectorIndex);
            chainCount++;
        }
    }
    
    destroy = true;
}