#include "Headers.h"
#include "ComponentUICheckbox.h"
#include "GameObject.h"
#include "MaterialComponent.h"

ComponentUICheckbox::ComponentUICheckbox(GameObject* gameObject) : ComponentUI(gameObject)
{
	State = CheckboxState::NORMAL;
	_type = Component::Type::UI_CHECKBOX;
}

ComponentUICheckbox::~ComponentUICheckbox()
{
}

void ComponentUICheckbox::InputUpdate()
{
	// Add here any checks necessary with INPUT.

	// PROBLEMA A SOLUCIONAR: NO PODEM TENIR ACTIVE I HOVERED AL MATEI TEMPS (EN PROCES DE PENSAR UNA SOLUCIÓ) 

	State = ChangeState(State);
	
	switch (State)
	{
	case CheckboxState::NORMAL:
		Console::S_Log("Im in Disble");
		//LOG("Im in Normal Mode");
		break;
	case CheckboxState::HOVERED:
		Console::S_Log("Im Hovered");
		break;
	case CheckboxState::HOVEREDACTIVE:
		Console::S_Log("Im Hovered and Active");
		break;
	case CheckboxState::ONPRESS:
		Console::S_Log("Im get Presed");
		//LOG("Im get Presed");
		break;
	case CheckboxState::ACTIVE:
		Console::S_Log("Im Active");
		//LOG("Im get Presed");
		break;
	default:
		break;
	}


}

void ComponentUICheckbox::Serialization(json& j)
{
	json _j;

	_j["Type"] = _type;
	_j["MaterialResource"] = _material->GetResourceUID();
	_j["Enabled"] = _isEnabled;
	_j["State"] = State;
	//_j["checkActive"] = checkActive;
	j["Components"].push_back(_j);
}

void ComponentUICheckbox::DeSerialization(json& j)
{
	_material->ChangeTexture((ResourceTexture*)ModuleResourceManager::S_LoadResource(j["MaterialResource"]));

	bool enabled = j["Enabled"];
	if (!enabled)
		Disable();

	_gameObject->transform->ForceUpdate();

	State = j["State"];
	//checkActive = j["checkActive"];
}

CheckboxState ComponentUICheckbox::ChangeState(CheckboxState State)
{
	if (IsMouseOver()) {
		if (ModuleInput::S_GetMouseButton(1) != KEY_DOWN && State != CheckboxState::HOVEREDACTIVE && State != CheckboxState::ACTIVE)
		{
			State = CheckboxState::HOVERED;
		}

		if (ModuleInput::S_GetMouseButton(1) == KEY_UP)
		{
			State = CheckboxState::ONPRESS;

			if (checkActive == false)
			{
				checkActive = true;
			}

			else if (checkActive == true)
			{
				checkActive = false;
			}

		}

		if (State == CheckboxState::ACTIVE)
		{
			State = CheckboxState::HOVEREDACTIVE;
		}
	}

	if (State == CheckboxState::ONPRESS)
	{
		if (checkActive == true)
			State = CheckboxState::ACTIVE;

		else if (checkActive == false)
			State = CheckboxState::NORMAL;
	}

	if (!IsMouseOver())
	{
		if (State != CheckboxState::HOVEREDACTIVE && State != CheckboxState::ACTIVE)
		{
			State = CheckboxState::NORMAL;
		}

		if (State == CheckboxState::HOVEREDACTIVE)
		{
			State = CheckboxState::ACTIVE;
		}
	}

	return State;
}

#ifdef STANDALONE
void ComponentUICheckbox::OnEditor()
{
	bool created = true;
	if (!ImGui::CollapsingHeader("Check Box", &created, ImGuiTreeNodeFlags_DefaultOpen)) return;
	if (!created)
	{
		_gameObject->DestroyComponent(this);
		return;
	}

	/*bool auxiliaryBool = _isEnabled;
	if (ImGui::Checkbox("Active##Material", &auxiliaryBool))
		auxiliaryBool ? Enable() : Disable();*/

	ImGui::Text("Im a CHECK BOX");
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
}
#endif // STANDALONE