#include "Headers.h"
#include "GameObject.h"
#include "ModuleLayers.h"
#include "TransformComponent.h"
#include "MeshRenderComponent.h"
#include "MaterialComponent.h"

GameObject::GameObject(GameObject* parent, std::string name, std::string tag) : name(name), tag(tag)
{
	_ID = Application::Instance()->layers->AddGameObject(this);
	transform = AddComponent<TransformComponent>();
	if (parent != nullptr) parent->AddChild(this);

}

GameObject::GameObject(GameObject* parent, std::string& name, std::string& tag) : name(name), tag(tag)
{
	_ID = Application::Instance()->layers->AddGameObject(this);
	transform = AddComponent<TransformComponent>();
	if (parent != nullptr) parent->AddChild(this);
}

GameObject::~GameObject()
{
	for (int i = 0; i < _components.size(); i++)
	{
		RELEASE(_components[i]);
	}
	_components.clear();

	for (int i = 0; i < _children.size(); i++)
	{
		RELEASE(_children[i]);
	}
	_children.clear();
}

bool GameObject::AddChild(GameObject* child)
{
	if (!child) return false;
	if (child->_parent == this) return false;

	GameObject* p = _parent;

	while (p)
	{
		if (p == child) return false;

		p = p->_parent;
	}

	_children.push_back(child);

	if (child->_parent) child->_parent->RemoveChild(child);

	child->_parent = this;

	child->transform->parentGlobalTransform = transform->GetGlobalTransform(); // Set the child parentGlobalTransform to this trasnform's global transfrom.
	child->transform->UpdateTransform();
	return true;
}

bool GameObject::SetParent(GameObject* parent)
{
	return parent->AddChild(this);
}

void GameObject::SetActive(bool active)
{
	_isActive = active;

	for (auto* component : _components)
	{
		component->Enable(active);
	}

	for (auto* child : _children)
	{
		child->SetActive(active);
	}

}

void GameObject::OnEditor()
{
	for (auto* component : _components)
	{
		component->OnEditor();
	}

	ImGui::Separator();

	ImGui::Spacing();
	if (ImGui::BeginCombo("Add Component", "Select"))
	{
		for (int n = 0; n < 2; n++)
		{
			int selectedItem = n;
			if (ImGui::Selectable(comboValues[n].c_str(), false))
			{
				switch (n)
				{
				case 0:
					Console::S_Log("Cannot add empty MeshRenderComponents yet!", true);
					//AddComponent<MeshRenderComponent>();
					break;
				case 1:
					if (!HasComponent<MaterialComponent>())
						AddComponent<MaterialComponent>();
					else
						Console::S_Log("Cannot add two Materials to a single GameObject.", true);
					break;
				}
			}
		}
		ImGui::EndCombo();
	}

}

void GameObject::RemoveChild(GameObject* child)
{
	if (!child) return;

	for (int i = 0; i < _children.size(); ++i)
	{
		if (_children[i] == child) _children.erase(_children.begin() + i);
	}
	child->_parent = nullptr;
}
