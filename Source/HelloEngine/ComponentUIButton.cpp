#include "Headers.h"
#include "ComponentUIButton.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "TextureImporter.h"
#include "Mesh.h"
#include "API/API_UIButton.h"
#include "ComponentUICheckbox.h"
#include "ComponentUISlider.h"

ComponentUIButton::ComponentUIButton(GameObject* gameObject) : ComponentUI(gameObject)
{
    State = ButtonState::NORMAL;
    _type = Component::Type::UI_BUTTON;

    uiObject = gameObject;
}

ComponentUIButton::~ComponentUIButton()
{
}

void ComponentUIButton::InputUpdate()
{
    // Add here any checks necessary with INPUT.
    //State = ChangeState(State);

    switch (State)
    {
    case ButtonState::NORMAL:
        _material->ChangeTexture(textureIDIdle);
        gameTimeCopy = EngineTime::GameTimeCount();
        break;
    case ButtonState::HOVERED:
        _material->ChangeTexture(textureIDHover);
        gameTimeCopy = EngineTime::GameTimeCount();
        break;
    case ButtonState::ONPRESS:
        _material->ChangeTexture(textureIDPress);
        /*if (EngineTime::GameTimeCount() >= gameTimeCopy + 0.5)
        {
            State = ButtonState::ONHOLD;
        }*/
        break;
    case ButtonState::ONHOLD:
        //State = ButtonState::HOVERED;
        break;
    case ButtonState::BLOCKED:
        _material->ChangeTexture(textureIDBlocked);
        isBlocked = true;
        gameTimeCopy = EngineTime::GameTimeCount();
        break;
    default:
        break;
    }
}

void ComponentUIButton::Serialization(json& j)
{
    json _j;

    _j["Type"] = _type;
    _j["MaterialResource"] = _material->GetResourceUID();
    _j["Enabled"] = _isEnabled;
    _j["State"] = State;
    _j["idleImage"] = idleButton ? idleButton->UID : 0;
    _j["hoverImage"] = hoverButton ? hoverButton->UID : 0;
    _j["pressImage"] = pressButton ? pressButton->UID : 0;
    _j["blockImage"] = blockedButton ? blockedButton->UID : 0;
    j["Components"].push_back(_j);
}

void ComponentUIButton::DeSerialization(json& j)
{
    _material->ChangeTexture((ResourceTexture*)ModuleResourceManager::S_LoadResource(j["MaterialResource"]));

    bool enabled = j["Enabled"];
    if (!enabled)
        Disable();

    _gameObject->transform->ForceUpdate();

    uint savedUIDIdle = j["idleImage"];
    idleButton = savedUIDIdle == 0 ? nullptr : (ResourceTexture*)ModuleResourceManager::S_LoadResource(j["idleImage"]);
    if (idleButton != nullptr)
        textureIDIdle = idleButton->OpenGLID;
    else
        textureIDIdle = -1;

    uint savedUIDHover = j["hoverImage"];
    hoverButton = savedUIDHover == 0 ? nullptr : (ResourceTexture*)ModuleResourceManager::S_LoadResource(j["hoverImage"]);
    if (hoverButton != nullptr)
        textureIDHover = hoverButton->OpenGLID;
    else
        textureIDHover = -1;

    uint savedUIDPress = j["pressImage"];
    pressButton = savedUIDPress == 0 ? nullptr : (ResourceTexture*)ModuleResourceManager::S_LoadResource(j["pressImage"]);
    if (pressButton != nullptr)
        textureIDPress = pressButton->OpenGLID;
    else
        textureIDPress = -1;

    if (j.contains("blockImage"))
    {
        uint savedUIDBlocked = j["blockImage"];
        blockedButton = savedUIDBlocked == 0 ? nullptr : (ResourceTexture*)ModuleResourceManager::S_LoadResource(j["blockImage"]);
        if (blockedButton != nullptr)
            textureIDBlocked = blockedButton->OpenGLID;
        else
            textureIDBlocked = -1;
    }

    State = j["State"];

    switch (State)
    {
    case ButtonState::NORMAL:
        _material->ChangeTexture(idleButton);
        break;
    case ButtonState::HOVERED:
        _material->ChangeTexture(hoverButton);
        break;
    case ButtonState::ONPRESS:
        _material->ChangeTexture(pressButton);
        break;
    case ButtonState::ONHOLD:
        _material->ChangeTexture(pressButton);
        break;
    case ButtonState::BLOCKED:
        _material->ChangeTexture(blockedButton);
        break;
    default:
        break;
    }
}

ButtonState ComponentUIButton::ChangeState(ButtonState State)
{

    if (IsMouseOver()) {
        //esta seleccionat
        if (State != ButtonState::ONHOLD && ModuleInput::S_GetMouseButton(1) != KEY_REPEAT)
        {

        }

        //ha sigut clicat
        if (ModuleInput::S_GetMouseButton(1) == KEY_UP && State != ButtonState::ONHOLD)
        {
            State = ButtonState::ONPRESS;
        }
        //esta sent mantenit clickat
        if (ModuleInput::S_GetMouseButton(1) == KEY_DOWN && State != ButtonState::ONHOLD)
        {

        }
        if (ModuleInput::S_GetMouseButton(1) == KEY_REPEAT)
        {

        }

        else if (State == ButtonState::ONHOLD)
        {

        }
    }

    if (!IsMouseOver() && State != ButtonState::NORMAL)
    {

    }

    return State;
}

void ComponentUIButton::UpdateGamePadInput(bool selected)
{
    isPress = false;
    isReleased = false;

    if (selected)
    {
        Console::S_Log((int)State);
        if (ModuleInput::S_GetGamePadButton(GamePad::BUTTON_A) == KEY_DOWN && State != ButtonState::ONPRESS)
        {
            ModuleInput::S_HandleGamePadButton(GamePad::BUTTON_A); // Handle A button so no other UI detects it.
            State = ButtonState::ONPRESS;
            isPress = true;
        }
        else if (ModuleInput::S_GetGamePadButton(GamePad::BUTTON_A) == KEY_REPEAT)
        {
            Console::S_Log("hiold");
            State = ButtonState::ONHOLD;
        }
        else
        {
            // If previous state was hold or press, this frame the button was released
            if (ModuleInput::S_GetGamePadButton(GamePad::BUTTON_A) == KEY_UP && (State == ButtonState::ONHOLD || State == ButtonState::ONPRESS))
            {
                isReleased = true;
            }
                
            State = ButtonState::HOVERED;
        }
    }
    else
    {
        State = ButtonState::NORMAL;
    }

    if (isBlocked)
        State = ButtonState::BLOCKED;

    switch (State)
    {
    case ButtonState::NORMAL:
        _material->ChangeTexture(textureIDIdle);
        break;
    case ButtonState::HOVERED:
        _material->ChangeTexture(textureIDHover);
        break;
    case ButtonState::ONPRESS:
    case ButtonState::ONHOLD:
        _material->ChangeTexture(textureIDPress);
        break;
    case ButtonState::BLOCKED:
        _material->ChangeTexture(textureIDBlocked);
        break;
    default:
        break;
    }
}

#ifdef STANDALONE
void ComponentUIButton::OnEditor()
{

    bool created = true;
    if (!ImGui::CollapsingHeader("Button", &created, ImGuiTreeNodeFlags_DefaultOpen)) return;
    if (!created)
    {
        _gameObject->DestroyComponent(this);
        return;
    }

    if (ImGui::Checkbox("Button Blocked##Button", &isBlockedInspector))
        isBlockedInspector ? isBlocked = true : isBlocked = false;


    ImGui::Text("States Colors:");

    ImGui::Text("NORMAL"); ImGui::SameLine();
    ImGui::ColorEdit4("color", colors);
    //ImGui::Text("HOVERED");
    //ImGui::SameLine();
    //ImGui::ColorEdit3("color", colors);
    //ImGui::Text("ONPRESS");
    //ImGui::SameLine();
    //ImGui::ColorEdit3("color", colors);
    //ImGui::Text("ONHOLD");
    //ImGui::SameLine();
    //ImGui::ColorEdit3("color", colors);
    ImGui::Separator();
    ImGui::Text("States Textures:");
    ImGui::Text("Normal:"); ImGui::SameLine();

    {
        std::string imageName;
        int width = 0;
        int height = 0;

        if (textureIDIdle != -1.0f && idleButton != nullptr)
        {
            ImGui::Image((ImTextureID)(uint)textureIDIdle, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));

            imageName = idleButton->debugName;
            width = idleButton->width;
            height = idleButton->height;
        }
        else
        {
            ImGui::Image((ImTextureID)0, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));
            imageName = "None";
        }

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Texture"))
            {
                //Drop asset from Asset window to scene window
                const uint* drop = (uint*)payload->Data;

                idleButton = (ResourceTexture*)ModuleResourceManager::S_LoadResource(*drop);
                textureIDIdle = idleButton->OpenGLID;
                _material->ChangeTexture(idleButton);
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::TextWrapped("Path: "); ImGui::SameLine();
        ImGui::TextColored(ImVec4(1, 1, 0, 1), imageName.c_str());

        ImGui::TextWrapped("Width: "); ImGui::SameLine();
        ImGui::TextColored(ImVec4(1, 1, 0, 1), std::to_string(width).c_str());

        ImGui::TextWrapped("Height: "); ImGui::SameLine();
        ImGui::TextColored(ImVec4(1, 1, 0, 1), std::to_string(height).c_str());
    }

    //////////////////////////////////////////////////////////////////////////////////////
    ImGui::Text("");
    ImGui::Text("");
    ImGui::Text("Hovered:"); ImGui::SameLine();
    {
        //Mesh& mesh = _material->GetMesh();

        std::string imageName;
        int width = 0;
        int height = 0;

        if (textureIDHover != -1.0f && hoverButton != nullptr)
        {
            ImGui::Image((ImTextureID)(uint)textureIDHover, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));

            imageName = hoverButton->debugName;
            width = hoverButton->width;
            height = hoverButton->height;
        }
        else
        {
            ImGui::Image((ImTextureID)0, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));
            imageName = "None";
        }

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Texture"))
            {
                //Drop asset from Asset window to scene window
                const uint* drop = (uint*)payload->Data;

                hoverButton = (ResourceTexture*)ModuleResourceManager::S_LoadResource(*drop);
                textureIDHover = hoverButton->OpenGLID;
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::TextWrapped("Path: "); ImGui::SameLine();
        ImGui::TextColored(ImVec4(1, 1, 0, 1), imageName.c_str());

        ImGui::TextWrapped("Width: "); ImGui::SameLine();
        ImGui::TextColored(ImVec4(1, 1, 0, 1), std::to_string(width).c_str());

        ImGui::TextWrapped("Height: "); ImGui::SameLine();
        ImGui::TextColored(ImVec4(1, 1, 0, 1), std::to_string(height).c_str());
    }
    ///////////////////////////////////////////////////////////////////////////////////////////
    ImGui::Text("");
    ImGui::Text("");
    ImGui::Text("Press:"); ImGui::SameLine();
    {
        //Mesh& mesh = _material->GetMesh();

        std::string imageName;
        int width = 0;
        int height = 0;

        if (textureIDPress != -1.0f && pressButton != nullptr)
        {
            ImGui::Image((ImTextureID)(uint)textureIDPress, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));

            imageName = pressButton->debugName;
            width = pressButton->width;
            height = pressButton->height;
        }
        else
        {
            ImGui::Image((ImTextureID)0, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));
            imageName = "None";
        }

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Texture"))
            {
                //Drop asset from Asset window to scene window
                const uint* drop = (uint*)payload->Data;

                pressButton = (ResourceTexture*)ModuleResourceManager::S_LoadResource(*drop);
                textureIDPress = pressButton->OpenGLID;
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::TextWrapped("Path: "); ImGui::SameLine();
        ImGui::TextColored(ImVec4(1, 1, 0, 1), imageName.c_str());

        ImGui::TextWrapped("Width: "); ImGui::SameLine();
        ImGui::TextColored(ImVec4(1, 1, 0, 1), std::to_string(width).c_str());

        ImGui::TextWrapped("Height: "); ImGui::SameLine();
        ImGui::TextColored(ImVec4(1, 1, 0, 1), std::to_string(height).c_str());
    }

    ///////////////////////////////////////////////////////////////////////////////////////////
    ImGui::Text("");
    ImGui::Text("");
    ImGui::Text("Block:"); ImGui::SameLine();
    {
        //Mesh& mesh = _material->GetMesh();

        std::string imageName;
        int width = 0;
        int height = 0;

        if (textureIDBlocked != -1.0f && blockedButton != nullptr)
        {
            ImGui::Image((ImTextureID)(uint)textureIDBlocked, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));

            imageName = blockedButton->debugName;
            width = blockedButton->width;
            height = blockedButton->height;
        }
        else
        {
            ImGui::Image((ImTextureID)0, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));
            imageName = "None";
        }

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Texture"))
            {
                //Drop asset from Asset window to scene window
                const uint* drop = (uint*)payload->Data;

                blockedButton = (ResourceTexture*)ModuleResourceManager::S_LoadResource(*drop);
                textureIDBlocked = blockedButton->OpenGLID;
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::TextWrapped("Path: "); ImGui::SameLine();
        ImGui::TextColored(ImVec4(1, 1, 0, 1), imageName.c_str());

        ImGui::TextWrapped("Width: "); ImGui::SameLine();
        ImGui::TextColored(ImVec4(1, 1, 0, 1), std::to_string(width).c_str());

        ImGui::TextWrapped("Height: "); ImGui::SameLine();
        ImGui::TextColored(ImVec4(1, 1, 0, 1), std::to_string(height).c_str());
    }
}

#endif // STANDALONE