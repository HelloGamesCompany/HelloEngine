#include "Headers.h"
#include "CommandSetParentGameObject.h"
#include "GameObject.h"

CommandSetParentGameObject::CommandSetParentGameObject(GameObject* gameObject, GameObject* newParent)
{
	_gameObject = gameObject;
	_anotherParent = _gameObject->_parent;
	_gameObject->SetParent(newParent);
}

CommandSetParentGameObject::~CommandSetParentGameObject()
{
}

void CommandSetParentGameObject::Undo()
{
	GameObject* temp = _gameObject->_parent;
	_gameObject->SetParent(_anotherParent);
	_anotherParent = temp;
}

void CommandSetParentGameObject::Redo()
{
	GameObject* temp = _gameObject->_parent;
	_gameObject->SetParent(_anotherParent);
	_anotherParent = temp;
}
