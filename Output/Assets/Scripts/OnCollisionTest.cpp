#include "OnCollisionTest.h"
HELLO_ENGINE_API_C OnCollisionTest* CreateOnCollisionTest(ScriptToInspectorInterface* script)
{
	OnCollisionTest* classInstance = new OnCollisionTest();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void OnCollisionTest::Start()
{

}
void OnCollisionTest::Update()
{

}
void OnCollisionTest::OnCollisionEnter(API::API_RigidBody other)
{
	std::string thisName = gameObject.GetName();
	std::string otherName = other.GetGameObject().GetName();
	Console::Log("Enter: " + thisName + " with " + otherName);
}

void OnCollisionTest::OnCollisionStay(API::API_RigidBody other)
{
	std::string thisName = gameObject.GetName();
	std::string otherName = other.GetGameObject().GetName();
	Console::Log("Stay: " + thisName + " with " + otherName);
}

void OnCollisionTest::OnCollisionExit(API::API_RigidBody other)
{
	std::string thisName = gameObject.GetName();
	std::string otherName = other.GetGameObject().GetName();
	Console::Log("Exit: " + thisName + " with " + otherName);
}

