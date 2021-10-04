#include "ModuleInput.h"
#include "Application.h"
#include <iostream>


bool ModuleInput::Update()
{
	// Input throw window console
	getline(cin, currentLoopInput);

	// I don't know what could go wrong receiving input
	return true;
}
