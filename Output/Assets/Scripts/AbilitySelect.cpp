#include "AbilitySelect.h"
HELLO_ENGINE_API_C AbilitySelect* CreateAbilitySelect(ScriptToInspectorInterface* script)
{
    AbilitySelect* classInstance = new AbilitySelect();

    script->AddDragBoxUIInput("Main Panel", &classInstance->mainPanel);
    script->AddDragBoxUIInput("Current Panel", &classInstance->abilityPanel);
    script->AddDragBoxUIButton("AbilityButton", &classInstance->abilitySelect);
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    return classInstance;
}

void AbilitySelect::Start()
{
    abilityPanel.SetEnable(false);
}

void AbilitySelect::Update()
{
    if (abilitySelect.OnHovered() && mainPanel.IsEnabled())
    {
        waitFrame = false;
    }
    else if (abilitySelect.OnPress() && !waitFrame)
    {
        mainPanel.SetEnable(false);
        abilityPanel.SetEnable(true);
        waitFrame = true;
    }
    else if (!abilitySelect.OnHovered())
    {
        waitFrame = true;
    }
}