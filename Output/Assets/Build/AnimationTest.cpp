#include "AnimationTest.h"
HELLO_ENGINE_API_C AnimationTest* CreateAnimationTest(ScriptToInspectorInterface* script)
{
	AnimationTest* classInstance = new AnimationTest();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxAnimationPlayer("AnimationPlayer", &classInstance->anim);
	script->AddDragBoxAnimationResource("Walk Animation", &classInstance->animWalk);

	return classInstance;
}

void AnimationTest::Start()
{
	anim.Play();   
}
void AnimationTest::Update()
{
	if (Input::GetKey(KeyCode::KEY_A) == KeyState::KEY_DOWN)
	{
		if (animWalk != 0)
		{
			anim.ChangeAnimation(animWalk);
			anim.Play();
		}
	}
}