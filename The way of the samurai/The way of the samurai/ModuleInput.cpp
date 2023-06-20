#include "ModuleInput.h"
#include "Application.h"
#include "ModuleGameImporter.h"
#include "ModuleLocalization.h"
#include <iostream>


bool ModuleInput::Update()
{
	bool validInputProvided = false;
	while (!validInputProvided)
	{
		validInputProvided = true;

		// Input through window console
		getline(cin, currentLoopInput);

		// Iterate invalid chars
		for (int i = 0; i < MAX_INVALID_CHARS; i++)
		{
			// Check if the current input contains an invalid character
			if (currentLoopInput.find(invalidCharacters[i]) != string::npos) 
			{
				// Localize invalid character text
				string invalidCharacterText = app->gameImporter->config->invalidCharacterText;
				app->localization->HandleLocalization(invalidCharacterText);

				// Assemble message
				string invalidCharacterMessage = invalidCharacterText + ": " + invalidCharacters[i];

				// Notify the player about the invalid character
				app->log(invalidCharacterMessage.c_str());

				// Ask for input again
				validInputProvided = false;
				break;
			}
		}
	}

	// I don't know what could go wrong receiving input
	return true;
}
