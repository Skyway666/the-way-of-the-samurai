#include "ModuleInput.h"
#include "Application.h"

bool ModuleInput::Update()
{
	// Intruction to the user
	app->log("Press 'e' + 'enter' to exit");

	// Exit if the user presses 'e'
	bool ret = true;
	if (getchar() == 'e')
		ret = false;

	return ret;
}
