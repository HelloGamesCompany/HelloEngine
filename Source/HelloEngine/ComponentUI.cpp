#include "Headers.h"
#include "ComponentUI.h"
#include "MeshRenderComponent.h"
#include "TextureComponent.h"
#include "GameObject.h"
#include "LayerEditor.h"
#include "ImWindowGame.h"
#include "LayerUI.h"

//TEMPORAL
#include "ModuleLayers.h"

ComponentUI::ComponentUI(GameObject* gameObject) : Component(gameObject)
{
    _type = Component::Type::UI;

    // Create a MeshRenderComponent2D and a MaterialComponent.
    _meshRenderer = gameObject->AddComponent<MeshRenderComponent>();
    _material = gameObject->AddComponent<TextureComponent>();

    // Notify both Mesh renderer and Material that they are handled by a UI Component. This makes them avoid certain behaviors that are not suitable for UI.
    _meshRenderer->SetAs2D(); 
    _material->SetAsUI();

#ifdef STANDALONE
    _gameWindow = (ImWindowGame*)LayerEditor::_imWindows[(uint)ImWindowID::GAME];
#endif // STANDALONE

    _UUID = HelloUUID::GenerateUUID();

    ModuleLayers::layerUI->AddUI(_UUID, this);
}

ComponentUI::~ComponentUI()
{
    ModuleLayers::layerUI->RemoveUI(_UUID);
}

bool ComponentUI::IsMouseOver()
{
    float3 globalScale = _gameObject->transform->GetGlobalScale();
    float3 globalPos = _gameObject->transform->GetGlobalPosition();

#ifndef STANDALONE
    float windowWidth = ModuleWindow::width;
    float windowHeight = ModuleWindow::height;
#else
    float windowWidth = _gameWindow->gameWidth + 15;
    float windowHeight = _gameWindow->gameHeight;
#endif
    int positionX = globalPos.x * (windowWidth * 0.5f);
    int positionY = -globalPos.y * (windowHeight * 0.5f);
    int width = globalScale.x * windowWidth;
    int height = globalScale.y * windowHeight;

    positionX += (1-globalScale.x) * windowWidth * 0.5f;
    positionY += (1 - globalScale.y) * windowHeight * 0.5f;


#ifdef STANDALONE
    // Get mouse position normalized inside this window-----------------------------------------------------
    ImVec2 windowPos = { (float)_gameWindow->gamePosX, (float)_gameWindow->gamePosY };

    ImVec2 normalizedPos = { (float)ModuleInput::S_GetMouseX(), (float)ModuleInput::S_GetMouseY() };

    normalizedPos = { normalizedPos.x - windowPos.x,  normalizedPos.y - windowPos.y };

    float mousePosX = normalizedPos.x;
    float mousePosY = normalizedPos.y;

    mousePosY -= 18; // Debugging code. ImGui gives an incorrect height.

#else
    float mousePosX = (float)ModuleInput::S_GetMouseX();
    float mousePosY = (float)ModuleInput::S_GetMouseY();
#endif // STANDALONE

    // Check if the mouse is over the rectangle.
    if (mousePosX < positionX || mousePosX > positionX + width || mousePosY < positionY || mousePosY > positionY + height)
        return false;

#ifdef STANDALONE
    // Check if the mouse is outside the bounds of the window.
    if (mousePosX > windowWidth || mousePosY > windowHeight || mousePosX < 0 || mousePosY < 0)
        return false;
#endif // STANDALONE

    //Console::S_Log("Mouse Over!");
    return true;

}

void ComponentUI::Serialization(json& j)
{
    json _j;

    _j["Type"] = _type;
    _j["MaterialResource"] = _material->GetResourceUID();
    _j["Enabled"] = _isEnabled;
    j["Components"].push_back(_j);
}

void ComponentUI::DeSerialization(json& j)
{
    _material->ChangeTexture((ResourceTexture*)ModuleResourceManager::S_LoadResource(j["MaterialResource"]));

    bool enabled = j["Enabled"];
    if (!enabled)
        Disable();

    _gameObject->transform->ForceUpdate();
}

void ComponentUI::SaveMeshState(json& j)
{
    j["MeshEnabled"] = _meshRenderer->IsEnabled();
}

void ComponentUI::LoadMeshState(json& j)
{
    if (j.contains("MeshEnabled"))
    {
        bool enabled = j["MeshEnabled"];

        if (!enabled)
        {
            _meshRenderer->Disable();
        }
    }
}
