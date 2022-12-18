#pragma once

#include "Command.h"

class GameObject;

class CommandSetParentGameObject : public Command
{
public:
	CommandSetParentGameObject(GameObject* gameObject, GameObject* newParent);

	~CommandSetParentGameObject();

	void Undo() override;

	void Redo() override;

private:
	GameObject* _gameObject = nullptr;

	GameObject* _anotherParent = nullptr;
};