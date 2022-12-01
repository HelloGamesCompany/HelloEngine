#include "Headers.h"
#include "GameObject.h"
#include "ModuleLayers.h"
#include "TransformComponent.h"
#include "MeshRenderComponent.h"
#include "MaterialComponent.h"
#include "CameraComponent.h"
#include "LayerEditor.h"

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
	// TODO: This won't work for now because of the Command system to Undo / Redo. It will be implemented in the future
	
	// Suspiciously similar to another Destroy() method in another person's repository...

	//if (Application::Instance()->layers->editor->GetSelectedGameObject() == this)
	//{
	//	Application::Instance()->layers->editor->SetSelectGameObject(nullptr);
	//}
	
	_parent->RemoveChild(this);
	
	Application::Instance()->layers->gameObjects[_ID] = nullptr;
	
	Application::Instance()->layers->deletedGameObjects.push_back(this);

	for (int i = 0; i < _children.size();)
	{
		_children[i]->Destroy();
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