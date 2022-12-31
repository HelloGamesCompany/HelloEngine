#include "Creationtest.h"
void Creationtest::Start()
{

}
void Creationtest::Update()
{
	std::string goName = testGameObject.GetName();	
	Console::Log("Game object name: " + goName);
}