#include "ParticleTest.h"
HELLO_ENGINE_API_C ParticleTest* CreateParticleTest(ScriptToInspectorInterface* script)
{
	ParticleTest* classInstance = new ParticleTest();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

	script->AddDragBoxParticleSystem("particletest", &classInstance->_particletest); 
	
	return classInstance; 
}

void ParticleTest::Start()
{

}
void ParticleTest::Update()
{
	if (Input::GetKey(KeyCode::KEY_P) == KeyState::KEY_DOWN)
	{
		_particletest.Play();
	}

	if (Input::GetKey(KeyCode::KEY_K) == KeyState::KEY_DOWN)
	{
		_particletest.Stop();
	}
}