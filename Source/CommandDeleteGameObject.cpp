#include "Headers.h"
#include "CommandDeleteGameObject.h"
#include "GameObject.h"

CommandDeleteGameObject::CommandDeleteGameObject(GameObject* gameObject)
{
	_gameObject = gameObject;
	_gameObject->MarkAsDead();
	_isGameObjectAlive = false;
}

CommandDeleteGameObject::~CommandDeleteGameObject()
{
	if(!_isGameObjectAlive && !Application::Instance()->IsExit())
	{
		_gameObject->Destroy();
	}

	_gameObject = nullptr;
}

void CommandDeleteGameObject::Undo()
{
	_isGameObjectAlive = true;
	_gameObject->MarkAsAlive();
}

void CommandDeleteGameObject::Redo()
{
	_isGameObjectAlive = false;
	_gameObject->MarkAsDead();
}