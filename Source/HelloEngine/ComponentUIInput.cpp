#include "Headers.h"
#include "ComponentUIInput.h"
#include "GameObject.h"
#include "MaterialComponent.h"
#include "ImWindowGame.h"
#include "LayerEditor.h"
#include "ModuleLayers.h"
ComponentUIInput::ComponentUIInput(GameObject* gameObject) : ComponentUI(gameObject)
{
	_type = Component::Type::UI_INPUT;

	_meshRenderer->Disable();

	_gameWindow = (ImWindowGame*)LayerEditor::_imWindows[(uint)ImWindowID::GAME];
}

ComponentUIInput::~ComponentUIInput()
{
}

void ComponentUIInput::InputUpdate()
{
	//detectar input
	if (_listButtons[ButtonSelected]->State == ButtonState::NORMAL) 
	{
		_listButtons[ButtonSelected]->State = ButtonState::ONHOLD;
		_listButtons[ButtonSelected]->IsHold = true;
	}
	else if (_listButtons[ButtonSelected]->IsHold == false) 
	{
		_listButtons[ButtonSelected]->State = ButtonState::NORMAL;
	}
	


	if (ButtonSelected < _listButtons.size() - 1 && ((ModuleInput::S_GetGamePadAxis(SDL_CONTROLLER_AXIS_LEFTY) < 128 && isPress) || ModuleInput::S_GetGamePadButton(GamePad::BUTTON_UP) == KEY_DOWN))
	{
		_listButtons[ButtonSelected]->IsHold = false;
		ButtonSelected++;
		Console::S_Log("Up");
		Console::S_Log(std::to_string(ButtonSelected));
		isPress = false;
	}
	else if (ButtonSelected > 0 && ((ModuleInput::S_GetGamePadAxis(SDL_CONTROLLER_AXIS_LEFTY) > 128 && isPress) || ModuleInput::S_GetGamePadButton(GamePad::BUTTON_DOWN) == KEY_DOWN))
	{
		_listButtons[ButtonSelected]->IsHold = false;
		ButtonSelected--;
		Console::S_Log(std::to_string(ButtonSelected));
		Console::S_Log("Down");
		isPress = false;
	}
	else if (ModuleInput::S_GetGamePadAxis(SDL_CONTROLLER_AXIS_LEFTY) == 128)
	{
		isPress = true;
	}
	//detectar el botton que estas y for�ar su nuevo estado
}

void ComponentUIInput::Serialization(json& j)
{
	json _j;

	_j["Type"] = _type;
	_j["MaterialResource"] = _material->GetResourceUID();
	_j["Enabled"] = _isEnabled;

	for (size_t i = 0; i < _listButtons.size(); i++)
	{
		_j["listButtons"][i] = _listButtons[i]->GetGameObject()->GetID();
	}

	j["Components"].push_back(_j);
}

void ComponentUIInput::DeSerialization(json& j)
{
	_material->ChangeTexture((ResourceTexture*)ModuleResourceManager::S_LoadResource(j["MaterialResource"]));

	bool enabled = j["Enabled"];
	if (!enabled)
		Disable();

	std::vector<uint> listAux = j["listButtons"];

	for (size_t i = 0; i < listAux.size(); i++)
	{
		GameObject* GOAux = ModuleLayers::gameObjects[listAux[i]];
		ComponentUIButton* CUIB = GOAux->GetComponent<ComponentUIButton>();

		if (CUIB != nullptr)
		{
			_listButtons.push_back(CUIB);
		}
	}

	_gameObject->transform->ForceUpdate();
}

#ifdef STANDALONE
void ComponentUIInput::OnEditor()
{
	bool created = true;
	if (!ImGui::CollapsingHeader("Panel", &created, ImGuiTreeNodeFlags_DefaultOpen)) return;
	if (!created)
	{
		_gameObject->DestroyComponent(this);
		return;
	}

	ImGui::Text("");
	ImGui::SameLine();

	if (ImGui::CollapsingHeader("List of Buttons"))
	{
		for (size_t i = 0; i < _listButtons.size() + 1; i++)
		{
			if (i != _listButtons.size())
			{
				ImGui::Text("   - ");
				ImGui::SameLine();
				ImGui::Button(_listButtons[i]->GetGameObject()->GetName().c_str());
			}
			else
			{
				ImGui::Text("   - ");
				ImGui::SameLine();
				ImGui::Button("Insert a button");
			}

			

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
				{
					//Drop asset from Asset window to scene window
					const uint* drop = (uint*)payload->Data;

					ComponentUIButton* UIB = ModuleLayers::gameObjects[*drop]->GetComponent<ComponentUIButton>();

					if (UIB != nullptr)
					{
						if (i != _listButtons.size())
						{
							_listButtons[i] = ModuleLayers::gameObjects[*drop]->GetComponent<ComponentUIButton>();
						}
						else
						{
							_listButtons.push_back(UIB);
						}
					}
				}
				ImGui::EndDragDropTarget();
			}
		}

		ImGui::Text("");

		if (ImGui::Button("Reset List"))
		{
			_listButtons.clear();
		}
	}

}
#endif // STANDALONE