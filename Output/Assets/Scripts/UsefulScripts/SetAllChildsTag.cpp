#include "SetAllChildsTag.h"
HELLO_ENGINE_API_C SetAllChildsTag* CreateSetAllChildsTag(ScriptToInspectorInterface* script)
{
    SetAllChildsTag* classInstance = new SetAllChildsTag();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddInputBox("Tag", &classInstance->tag);
    script->AddCheckBox("Set Tag For direct child only", &classInstance->onlyDirectChilds);
    return classInstance;
}

void SetAllChildsTag::Start()
{
    API_GameObject children[99];
    gameObject.GetChildren(children);
    for (size_t i = 0; i < 99; i++)
    {
        children[i].SetTag(tag.c_str());
    }
}

void SetAllChildsTag::Update()
{

}