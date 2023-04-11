#include "SaveTest.h"

using QSave = API::API_QuickSave;

HELLO_ENGINE_API_C SaveTest* CreateSaveTest(ScriptToInspectorInterface* script)
{
	SaveTest* classInstance = new SaveTest();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void SaveTest::Start()
{
	// Example for set value use QuickSave
	QSave::SetBool("testBool", true);
	QSave::SetFloat("testFloat", 3.1415925f);
	QSave::SetInt("testInt", 888);
	QSave::SetString("testString", "Hello Quick Save!");

	// temporal string for print
	std::string print;

	// Example for get value use QuickSave;
	bool tempB = QSave::GetBool("testBool");
	print = "testBool: ";
	print += tempB ? "true" : "false";
	Console::Log(print);

	float tempF = QSave::GetFloat("testFloat");
	print = "testFloat: " + std::to_string(tempF);
	Console::Log(print);

	float tempI = QSave::GetInt("testInt");
	print = "testInt: " + std::to_string(tempI);
	Console::Log(print);

	QSave::GetString("testString", print);
	print = "testString: " + print;
	Console::Log(print);

}
void SaveTest::Update()
{

}