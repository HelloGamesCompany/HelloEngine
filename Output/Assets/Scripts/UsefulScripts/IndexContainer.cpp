#include "IndexContainer.h"
HELLO_ENGINE_API_C IndexContainer* CreateIndexContainer(ScriptToInspectorInterface* script)
{
    IndexContainer* classInstance = new IndexContainer();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragInt("Index", &classInstance->index);
    script->AddInputBox("Play Audio Event String", &classInstance->playAudio);
    return classInstance;
}

void IndexContainer::Start()
{
     
}

void IndexContainer::Update()
{

}