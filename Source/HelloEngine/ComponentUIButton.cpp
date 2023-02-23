#include "Headers.h"
#include "ComponentUIButton.h"

ComponentUIButton::ComponentUIButton(GameObject* gameObject) : ComponentUI(gameObject)
{
}

ComponentUIButton::~ComponentUIButton()
{
}

void ComponentUIButton::InputUpdate()
{
	// Add here any checks necessary with INPUT.

	if (IsMouseOver())
		isFocused = true;

}
