#include "Headers.h"
#include "GameObject.h"
#include "ModuleLayers.h"
#include "TransformComponent.h"
#include "MeshRenderComponent.h"
#include "MaterialComponent.h"
#include "CameraComponent.h"
#include "LayerEditor.h"
#include "ImGuizmo/ImGuizmo.h"

GameObject::GameObject(GameObject* parent, std::string name, std::string tag, uint ID) : name(name), tag(tag)
{
	_ID = Application::Instance()->layers->AddGameObject(this, ID);
	transform = AddComponent<TransformComponent>();
	if (parent != nullptr)
		parent->AddChild(this);
}

GameObject::GameObject(GameObject* parent, std::string& name, std::string& tag, uint ID) : name(name), tag(tag)
{
	_ID = Application::Instance()->layers->AddGameObject(this, ID);
	transform = AddComponent<TransformComponent>();
	if (parent != nullptr) 
		parent->AddChild(this);
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

	if (child->_parent) 
		child->_parent->RemoveChild(child);

	child->_parent = this;

	transform->ForceUpdate();

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
		_isActive ? component->EnableFromGameObject() : component->DisableFromGameObject();
	}

	for (auto* child : _children)
	{
		child->SetActive(active);
	}

}

void GameObject::OnEditor()
{
	if (_isPendingToDelete) return;

	for (auto* component : _components)
	{
		component->OnEditor();
	}

	ImGui::Separator();

	ImGui::Spacing();
	if (ImGui::BeginCombo("Add Component", "Select"))
	{
		for (int n = 0; n < 3; n++)
		{
			int selectedItem = n;
			if (ImGui::Selectable(_comboValues[n].c_str(), false))
			{
				switch (n)
				{
				case 0:
					if (!HasComponent<MeshRenderComponent>())
						AddComponent<MeshRenderComponent>();
					break;
				case 1:
					if (!HasComponent<MaterialComponent>())
						AddComponent<MaterialComponent>();
					break;
				case 2:
					if (!HasComponent<CameraComponent>())
						AddComponent<CameraComponent>();
					break;
				}
			}
		}
		ImGui::EndCombo();
	}

	ImGui::TextColored(ImVec4(1, 1, 1, 0.5f), "GameObject UID: "); ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 1, 0.5f), std::to_string(_ID).c_str());
}

#ifdef STANDALONE
bool GameObject::MarkAsDead()
{
	if(!_isPendingToDelete)
	{
		if (Application::Instance()->layers->editor->selectedGameObject == this)
		{
			Application::Instance()->layers->editor->SetSelectGameObject(nullptr);
			ImGuizmo::Enable(false);
		}

		_isPendingToDelete = true;

		for (int i = 0; i < _children.size(); i++)
		{
			if(_children[i]->MarkAsDead())
			{
				_childrenDeletedIndex.push_back(i);
			}
		}

		for (int i = 0; i < _components.size(); i++)
		{
			_components[i]->MarkAsDead();
		}



		return true;
	}	

	return false;
}

bool GameObject::MarkAsAlive()
{
	if(_isPendingToDelete)
	{
		_isPendingToDelete = false;

		for (int i = 0; i < _childrenDeletedIndex.size(); i++)
		{
			_children[_childrenDeletedIndex[i]]->MarkAsAlive();
		}

		for (int i = 0; i < _components.size(); i++)
		{
			_components[i]->MarkAsAlive();
		}
	
		return true;
	}	

	return false;
}
#endif // STANDALONE

void GameObject::Destroy()
{
	if (_isDestroyed)
		return;

	if (Application::Instance()->layers->editor->selectedGameObject == this)
	{
		Application::Instance()->layers->editor->SetSelectGameObject(nullptr);
		ImGuizmo::Enable(false);
	}

	_isPendingToDelete = true;
	_isDestroyed = true;

	Application::Instance()->layers->gameObjects.erase(_ID);

	Application::Instance()->layers->_deletedGameObjects.push_back(this);

	for (int i = 0; i < _children.size(); i++)
	{
		_children[i]->Destroy();
	}
	_children.clear();
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

Component* GameObject::AddComponentOfType(Component::Type type)
{
	Component* newComponent = nullptr;
	switch (type)
	{
	case Component::Type::TRANSFORM:
		Console::S_Log("Cannot add another transform to a gameobject");
		return transform;
		break;
	case Component::Type::MESH_RENDERER:
		newComponent = new MeshRenderComponent(this);
		_components.push_back(newComponent);
		break;
	case Component::Type::MATERIAL:
		newComponent = new MaterialComponent(this);
		_components.push_back(newComponent);
		break;
	case Component::Type::CAMERA:
		newComponent = new CameraComponent(this);
		_components.push_back(newComponent);
		break;
	}

	return newComponent;
}

void GameObject::AddComponentSerialized(Component::Type type, json& jsonFile)
{
	Component* newComponent = AddComponentOfType(type);
	newComponent->DeSerialization(jsonFile);
}