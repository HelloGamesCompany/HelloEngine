#include "transformTest.h"
void transformTest::Start()
{

}
void transformTest::Update()
{
	if (Input::GetKey(KeyCode::KEY_W) == KeyState::KEY_REPEAT)
	{
		transfromTestVariable.Translate(0, 0, 0.2);
	}
	if (Input::GetKey(KeyCode::KEY_A) == KeyState::KEY_REPEAT)
	{
		transfromTestVariable.Translate(0.2, 0, 0);
	}
	if (Input::GetKey(KeyCode::KEY_S) == KeyState::KEY_REPEAT)
	{
		transfromTestVariable.Translate(0, 0, -0.2);
	}
	if (Input::GetKey(KeyCode::KEY_D) == KeyState::KEY_REPEAT)
	{
		transfromTestVariable.Translate(-0.2, 0, 0);
	}
}