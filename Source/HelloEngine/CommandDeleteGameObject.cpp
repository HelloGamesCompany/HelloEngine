#include "Headers.h"
#include "CommandDeleteGameObject.h"
#include "GameObject.h"

CommandDeleteGameObject::CommandDeleteGameObject(GameObject* gameObject)
{
	_gameObject = gameObject;
#if STANDALONE
	_gameObject->MarkAsDead();
#endif // STANDALONE


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
#if STANDALONE
	_gameObject->MarkAsAlive();
#endif // STANDALONE
}

void CommandDeleteGameObject::Redo()
{
	_isGameObjectAlive = false;
#if STANDALONE
	_gameObject->MarkAsDead();
#endif // STANDALONE
}