#include "ParticleCopyTest.h"
HELLO_ENGINE_API_C ParticleCopyTest* CreateParticleCopyTest(ScriptToInspectorInterface* script)
{
	ParticleCopyTest* classInstance = new ParticleCopyTest();
	script->AddDragBoxParticleSystem("Particle System", &classInstance->particleSystem);
	return classInstance;
}

void ParticleCopyTest::Start()
{
	myParticles = this->gameObject.AddParticleSystem(particleSystem);
}
void ParticleCopyTest::Update()
{
	if (Input::GetKey(KeyCode::KEY_A) == KeyState::KEY_DOWN)
	{
		myParticles.Play();
	}
}