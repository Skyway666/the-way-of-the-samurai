#include "ModuleGameLogic.h"
#include "Third Party/parson.h"
#include "ModuleGameImporter.h"
#include "ModuleInput.h"
#include "Application.h"
#include "ContainerUtils.h"

using namespace std;

bool ModuleGameLogic::Init()
{
	// TODO: Load game state from save file
	return true;
}

bool ModuleGameLogic::Start()
{
	// Initialize position with initial position
	gameState.currentGridPosition = app->gameImporter->config->initialPosition;

	// Log initial text
	logGameplayText(app->gameImporter->config->initialText);

	return true;
}

bool ModuleGameLogic::Update()
{
	// Loads the next event by reading input
	PlayerInputResult playerInputResult = HandlePlayerInput();

	// If an event was loaded, handle it
	if (playerInputResult == PlayerInputResult::EVENT_LOADED)
		HandleCurrentEvent();

	// Update went right if no fatal error was thrown
	return playerInputResult != PlayerInputResult::FATAL_ERROR;
}

ModuleGameLogic::PlayerInputResult ModuleGameLogic::HandlePlayerInput()
{
	PlayerInputResult ret = PlayerInputResult::FATAL_ERROR;

	// Try to load an event depending on the logic state
	switch (logicState)
	{
		// If the map is being navigated, update position
		case LogicState::NAVIGATING_MAP:
		{
			// If the moving command was invalid skip the logical frame
			if (!UpdateGridPosition())
			{
				ret = PlayerInputResult::INVALID_INPUT;
				break;
			}
		}
		// If the map is being navigated OR we are in an initialization phase, load map event
		case LogicState::LOAD_MAP_EVENT:
		{
			ret = LoadCurrentMapEvent();
			break;
		}
		// If there is an event to handle use the input to load a sub event
		case LogicState::BRANCHING_EVENT:
		{
			ret = HandleCurrentEventBranching();
			break;
		}

		case LogicState::SAVING_VARIABLE: 
		{
			// Variable saved
			SaveInputedVariable();

			// Ask for confirmation
			logicState = LogicState::SAVING_VARIABLE_CONFIRMATION;
			ret = PlayerInputResult::VARIABLE_LOADED;
			break;
		}
		case LogicState::SAVING_VARIABLE_CONFIRMATION:
		{
			ret = ConfirmSavedVariable();
			break;
		}
	}

	return ret;
}

void ModuleGameLogic::HandleCurrentEvent()
{
	// Display event text
	logGameplayText(handlingEvent->text);

	// Obtain conditions
	for (vector<string>::iterator it = handlingEvent->obtainedConditions.begin();
		it != handlingEvent->obtainedConditions.end();
		it++)
	{
		// The condition is not in the list so it can be added
		if (find(gameState.conditions.begin(), gameState.conditions.end(), (*it)) == gameState.conditions.end())
			gameState.conditions.push_back((*it));
	}

	// Remove conditions
	for (vector<string>::iterator it = handlingEvent->removedConditions.begin();
		it != handlingEvent->removedConditions.end();
		it++)
		gameState.conditions.remove((*it));

	// Obtain objects
	for (vector<string>::iterator it = handlingEvent->obtainedObjects.begin();
		it != handlingEvent->obtainedObjects.end();
		it++)
		gameState.objects.push_back((*it));

	// There is a variable to save: Priority 1
	if (handlingEvent->savedVariable != nullptr)
	{
		logicState = LogicState::SAVING_VARIABLE;
	}
	// There are sub events to handle: Priority 2
	else if (!handlingEvent->subEvents.empty())
	{
		HandleSubEventsDisplay(handlingEvent);
	}
	// There is nothing else to do
	else
	{
		// Go back to navigating the map
		BackToMap();
	}
}

ModuleGameLogic::PlayerInputResult ModuleGameLogic::LoadCurrentMapEvent()
{
	// Get map event
	MapEvent* currentMapEvent = GetCurrentMapEvent();

	// There is no current map event
	if (currentMapEvent == nullptr)
	{
		// Something went terribly wrong
		app->logFatalError("The grid position didn't had a matching map event");
		return PlayerInputResult::FATAL_ERROR;
	}
	// The map event is not navigable
	else if (!currentMapEvent->navigable)
	{
		logGameplayText(currentMapEvent->text);
		gameState.currentGridPosition = previousGridPosition;
		return PlayerInputResult::EVENT_ENDED;
	}
	// The map event is navigable
	else
	{
		// Load map event
		SetHandlingEvent(currentMapEvent);
		return PlayerInputResult::EVENT_LOADED;
	}
}

ModuleGameLogic::PlayerInputResult ModuleGameLogic::HandleCurrentEventBranching()
{
	// Check if the provided input leads to a sub event
	SubEvent* choosenOption = nullptr;
	for (vector<SubEvent>::iterator it = handlingEvent->subEvents.begin();
		it != handlingEvent->subEvents.end();
		it++)
	{
		if ((*it).option == app->input->currentLoopInput)
			choosenOption = &(*it);
	}

	// An option was choosen
	if (choosenOption != nullptr)
	{
		// Convert list of condition to a vector
		vector<string> gameConditionsVector = vector<string>(gameState.conditions.begin(), gameState.conditions.end());
		// Check if the conditions for the sub event are met
		bool conditionsMet = ContainerUtils::stringVectorAContainsB(gameConditionsVector, choosenOption->conditions);

		// If the conditions have been met
		if (conditionsMet)
		{
			// Load the sub event
			SetHandlingEvent(choosenOption);
			return PlayerInputResult::EVENT_LOADED;
		}
		// The conditions have not been met
		else
		{
			// Load the rejection text based on the missing conditions
			RejectionText* rejectionText = nullptr;
			for (vector<RejectionText>::iterator it = choosenOption->rejectionTexts.begin();
				it != choosenOption->rejectionTexts.end();
				it++)
			{
				// If the conditions for the rejection text are met
				if (ContainerUtils::stringVectorAContainsB(gameConditionsVector, (*it).conditions))
				{
					// Save the rejection text
					rejectionText = &(*it);
					break;
				}
			}

			// The rejection text was found
			if (rejectionText != nullptr)
			{
				// Display the rejection text
				logGameplayText(rejectionText->text);
			}
			// The rejection text was not found
			else
			{
				// Display default rejection text
				logGameplayText(app->gameImporter->config->defaultSubEventRejectionMessage);
			}

			// Go back to navigate the map
			BackToMap();
			return PlayerInputResult::EVENT_ENDED;
		}
	}
	// The choosen option was not avaliable
	else
	{
		// TODO: Include in config
		logGameplayText("That is not an option");
		return PlayerInputResult::INVALID_INPUT;
	}
}

void ModuleGameLogic::SaveInputedVariable()
{
	gameState.savedVariables[handlingEvent->savedVariable->key.c_str()] = app->input->currentLoopInput;

	// Display confirmation text
	logGameplayText(handlingEvent->savedVariable->confirmationText);

	// Display options
	vector<string> options = { "yes", "no" };
	DisplayOptions(options);
}

ModuleGameLogic::PlayerInputResult ModuleGameLogic::ConfirmSavedVariable()
{
	// User confirms the variable
	if (app->input->currentLoopInput == "yes")
	{
		// Display success to the user
		logGameplayText(handlingEvent->savedVariable->successText);

		// There is an event after saving variable
		if (handlingEvent->savedVariable->nextEvent != nullptr)
		{
			// Load next 
			SetHandlingEvent(handlingEvent->savedVariable->nextEvent);
			return PlayerInputResult::EVENT_LOADED;
		}
		// There is no next event
		else if (!handlingEvent->subEvents.empty())
		{
			// Handle the sub events
			HandleSubEventsDisplay(handlingEvent);
			return PlayerInputResult::EVENT_BRANCHED;
		}
		// There is no next event nor sub events
		else
		{
			// The event has ended, so go back to map
			BackToMap();
			return PlayerInputResult::EVENT_ENDED;
		}
	}

	// User doesn't confirm the variable
	else if (app->input->currentLoopInput == "no")
	{
		// Go back to the user writing the variable
		logGameplayText(handlingEvent->text);

		logicState = LogicState::SAVING_VARIABLE;
		return PlayerInputResult::VARIABLE_DISCARTED;
	}
	else
	{
		// Invalid input. TODO: Include in config
		logGameplayText("Invalid input");
		return PlayerInputResult::INVALID_INPUT;
	}
}

void ModuleGameLogic::HandleSubEventsDisplay(Event* eventToBranch)
{
	// Set the logic state so a sub event is selected
	logicState = LogicState::BRANCHING_EVENT;

	// Save options for the possible sub events
	vector<string> options;
	for (vector<SubEvent>::iterator it = handlingEvent->subEvents.begin();
		it != handlingEvent->subEvents.end();
		it++)
	{
		options.push_back((*it).option);
	}

	// Display options
	DisplayOptions(options);
}

void ModuleGameLogic::logGameplayText(string text) const
{
	// TODO 1: Treat it as a localization index by default. If it doesn't match any, use it as the final string
	
	// Add gameplay variables
	replaceVariables(text);

	// Display final text to the user
	app->log(text.c_str());
}

void ModuleGameLogic::replaceVariables(string& text) const
{
	int lastOcurrance = 0;

	// Iterate until all '@' are substituted
	while (text.find('@')!= string::npos) 
	{
		// Get position of the first and second @
		int openingAdd = text.find('@', lastOcurrance + 1);
		int closingAdd = text.find('@', openingAdd + 1);
		
		// Compute length of the variable key plus both @ at the beggining and the end
		int variableKeyLength = closingAdd - openingAdd;

		// Error handling
		if (closingAdd == string::npos) 
		{
			app->logFatalError("Variable without closing '@'");
			break;
		}

		// Get substring between @
		string variableKey = text.substr(openingAdd + 1, variableKeyLength - 1);

		// Check that the variable is in the dictionary
		if (gameState.savedVariables.find(variableKey) == gameState.savedVariables.end()) 
		{
			// Inform the user of the variable that wasn't found
			app->logFatalError(("Variable key: '" + variableKey + "' not found in variables dictionary").c_str());
			break;
		}

		// Finally, replace key with value
		text.replace(openingAdd, variableKeyLength + 1, gameState.savedVariables.at(variableKey));
	}
}

MapEvent* ModuleGameLogic::GetCurrentMapEvent() const
{
	// Iterate map events
	vector<MapEvent>* mapEvents = &app->gameImporter->mapEvents;
	for (vector<MapEvent>::iterator it = mapEvents->begin(); it != mapEvents->end(); it++) 
	{
		// Return pointer to map event with the current grid position
		if ((*it).gridPosition == gameState.currentGridPosition)
			return &(*it);
	}

	// No map event corresponds to this grid position
	return nullptr;
}

bool ModuleGameLogic::UpdateGridPosition()
{
	// Save previous position, in case we find a non navigatible tile
	previousGridPosition = gameState.currentGridPosition;

	// Grid movement
	bool ret = true;
	if (app->input->currentLoopInput == "north")
		gameState.currentGridPosition += app->gameImporter->config->gridRowLength;
	else if (app->input->currentLoopInput == "south")
		gameState.currentGridPosition -= app->gameImporter->config->gridRowLength;
	else if (app->input->currentLoopInput == "east")
		gameState.currentGridPosition ++;
	else if (app->input->currentLoopInput == "west")
		gameState.currentGridPosition--;
	// If the input is "reenter" we reproduce the same event again
	else if(app->input->currentLoopInput != "reenter")
	{
		ret = false;
		// TODO: Include in config
		logGameplayText("Invalid moving input, please introduce another command");
	}

	return ret;
}

void ModuleGameLogic::SetHandlingEvent(Event* newHandlingEvent)
{
	// Convert list of condition to a vector
	vector<string> gameConditionsVector = vector<string>(gameState.conditions.begin(), gameState.conditions.end());

	// Check if an alternative event should be loaded instead
	Event* alternativeEvent = nullptr;
	for (vector<AlternativeEvent>::iterator it = newHandlingEvent->alternativeEvents.begin();
		it < newHandlingEvent->alternativeEvents.end();
		it++) 
	{
		// Check if the conditions are met
		if (ContainerUtils::stringVectorAContainsB(gameConditionsVector, (*it).conditions)) 
		{
			alternativeEvent = (*it).alternative;
			break;
		}
	}

	// There is no alternative event
	if (alternativeEvent == nullptr)
		handlingEvent = newHandlingEvent;
	// There is an alternative event
	else
		handlingEvent = alternativeEvent;
}

void ModuleGameLogic::DisplayOptions(vector<string>& options) const
{
	// TODO: Introduce this text in the config file
	logGameplayText("Choose an option:");

	// Display options
	for(vector<string>::iterator it = options.begin(); it != options.end(); it++)
		app->log(("    -" + (*it)).c_str());
}

void ModuleGameLogic::BackToMap()
{
	// TODO: Include in config file
	logGameplayText("Where do you want to go?");
	logicState = LogicState::NAVIGATING_MAP;
}

void ModuleGameLogic::GameData::Save(JSON_Object* s_GameData)
{
	// TODO: Serialization of the game file
}


