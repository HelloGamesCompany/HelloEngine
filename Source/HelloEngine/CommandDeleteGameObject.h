#pragma once
#include "Command.h"

class GameObject;

class CommandDeleteGameObject : public Command
{
public:
	CommandDeleteGameObject(GameObject* gameObject);

	~CommandDeleteGameObject();
	
	void Undo() override;

	void Redo() override;

private:
	GameObject* _gameObject = nullptr;

	bool _isGameObjectAlive = false;
};