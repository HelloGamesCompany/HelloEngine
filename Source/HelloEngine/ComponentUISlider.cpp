#include "Headers.h"
#include "ComponentUISlider.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "ImWindowGame.h"
#include "LayerEditor.h"
#include "ComponentUIButton.h"
#include "ComponentUICheckbox.h"

ComponentUISlider::ComponentUISlider(GameObject* gameObject) : ComponentUI(gameObject)
{
	_type = Component::Type::UI_SLIDER;

	State = SliderState::NORMAL;
	
	_gameWindow = (ImWindowGame*)LayerEditor::_imWindows[(uint)ImWindowID::GAME];
}

ComponentUISlider::~ComponentUISlider()
{
}

void ComponentUISlider::InputUpdate()
{
	if (_gameObject->GetTag() == "UIsliderButton") 
	{
		if (IsMouseOver()) {
			isFocused = true;

			if (ModuleInput::S_GetMouseButton(1) == KEY_DOWN)
			{
				State = SliderState::ONPRESS;
			}
		}

		switch (State)
		{
		case SliderState::NORMAL:

			break;
		case SliderState::ONPRESS:
			if (ModuleInput::S_GetMouseButton(1) == KEY_REPEAT) 
			{
				if (ModuleInput::S_GetKey(SDL_SCANCODE_D) == KEY_DOWN) 
				{
					for (size_t i = 0; i < _gameObject->GetParent()->GetChildren()->size(); i++)
					{
						if (_gameObject->GetParent()->GetChildren()->at(i)->GetTag() == "UIsliderBar")
						{
							widthBar = _gameObject->GetParent()->GetChildren()->at(i)->transform->GetGlobalScale().x;
						}
					}

					if (mousePosX < widthBar)
					{
						mousePosX += 0.1f;
					}
				}
				else if (ModuleInput::S_GetKey(SDL_SCANCODE_A) == KEY_DOWN)
				{
					for (size_t i = 0; i < _gameObject->GetParent()->GetChildren()->size(); i++)
					{
						if (_gameObject->GetParent()->GetChildren()->at(i)->GetTag() == "UIsliderBar")
						{
							widthBar = _gameObject->GetParent()->GetChildren()->at(i)->transform->GetGlobalScale().x;
						}
					}

					if (mousePosX > -widthBar)
					{
						mousePosX -= 0.1f;
					}
				}
			}
			_gameObject->transform->SetPosition({ mousePosX, _gameObject->transform->GetGlobalScale().y, _gameObject->transform->GetGlobalScale().z });
			
			break;

		default:
			break;
		}

		if (!IsMouseOver())
			isFocused = false;

		if (ModuleInput::S_GetMouseButton(1) == KEY_UP)
		{
			State = SliderState::NORMAL;
		}
	}
}

void ComponentUISlider::UpdateGamePadInput(bool selected)
{
	
}

void ComponentUISlider::Serialization(json& j)
{
	json _j;

	_j["Type"] = _type;
	_j["MaterialResource"] = _material->GetResourceUID();
	_j["StateButton"] = State;
	_j["Enabled"] = _isEnabled;
	_j["MinValue"] = numMin;
	_j["MaxValue"] = numMax;
	_j["SliderPerCent"] = mousePosX;
	SaveMeshState(_j);
	j["Components"].push_back(_j);
}

void ComponentUISlider::DeSerialization(json& j)
{
	_material->ChangeTexture((ResourceTexture*)ModuleResourceManager::S_LoadResource(j["MaterialResource"]));

	State = j["StateButton"];

	numMin = j["MinValue"];
	numMax = j["MaxValue"];
	mousePosX = j["SliderPerCent"];

	LoadMeshState(j);

	_gameObject->transform->ForceUpdate();

	bool enabled = j["Enabled"];
	if (!enabled)
		Disable();

}

#ifdef STANDALONE
void ComponentUISlider::OnEditor()
{
	bool created = true;
	if (!ImGui::CollapsingHeader("Slider", &created, ImGuiTreeNodeFlags_DefaultOpen)) return;

	if (!created)
	{
		_gameObject->DestroyComponent(this);
		return;
	}

	if (_gameObject->GetTag() == "UIsliderButton") 
	{
		for (size_t i = 0; i < _gameObject->GetParent()->GetChildren()->size(); i++)
		{
			if (_gameObject->GetParent()->GetChildren()->at(i)->GetTag() == "UIsliderBar")
			{
				widthBarAux = _gameObject->GetParent()->GetChildren()->at(i)->transform->GetGlobalScale().x;
			}
		}

		perCent = ((mousePosX * 50) / (widthBarAux)) + 50;
		NormalizedPos = (((mousePosX * (numMax - numMin) /2) / widthBarAux) + ((numMax - numMin)/2)) + numMin;

		if (ImGui::SliderFloat("##SliderPerCent", &mousePosX, -widthBarAux, widthBarAux, "%.2f")) {
			_gameObject->transform->SetPosition({ mousePosX, _gameObject->transform->GetLocalPosition().y, _gameObject->transform->GetLocalPosition().z });
		}
		ImGui::SameLine(); ImGui::TextColored(ImVec4(1, 1, 0, 1), std::to_string(perCent).c_str());
		ImGui::SameLine(); ImGui::TextColored(ImVec4(1, 1, 0, 1), "H%");

		ImGui::TextColored(ImVec4(1, 1, 0, 1), std::to_string(NormalizedPos).c_str());


		ImGui::InputFloat("Min",&numMin);
		ImGui::InputFloat("Max",&numMax);
	}
}
#endif // STANDALONE