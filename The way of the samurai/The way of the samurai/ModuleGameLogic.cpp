#include "ModuleGameLogic.h"
#include "Third Party/parson.h"
#include "ModuleGameImporter.h"
#include "ModuleInput.h"
#include "Application.h"
#include "ContainerUtils.h"
#include <vector>

using namespace std;

bool ModuleGameLogic::Init()
{
	// TODO: Load game state from save file
	return true;
}

bool ModuleGameLogic::Start()
{
	// Initialize position with initial position
	gameState.currentGridPosition = app->gameImporter->config.initialPosition;

	// Log initial text
	app->log(app->gameImporter->config.initialText.c_str());

	// Ask player name if it is enabled
	if (app->gameImporter->config.savePlayerName) 
	{
		app->log(app->gameImporter->config.savePlayerNameText.c_str());
	}


	return true;
}

bool ModuleGameLogic::Update()
{
	// Loads the next event by reading input
	LoadEventResult loadedEventResult = LoadEvent();

	// Handle event if it could be loaded
	if (loadedEventResult == LoadEventResult::EVENT_LOADED)
		HandleCurrentEvent();

	return loadedEventResult != LoadEventResult::FATAL_ERROR;
}

void ModuleGameLogic::HandleCurrentEvent()
{
	// Display event text
	app->log(handlingEvent->text.c_str());

	// Obtain conditions
	for (vector<string>::iterator it = handlingEvent->obtainedConditions.begin();
		it != handlingEvent->obtainedConditions.end();
		it++)
		gameState.conditions.push_back((*it));

	// Obtain objects
	for (vector<string>::iterator it = handlingEvent->obtainedObjects.begin();
		it != handlingEvent->obtainedObjects.end();
		it++)
		gameState.objects.push_back((*it));

	// If there are no events to handle, go back to navigating the map
	if (handlingEvent->subEvents.empty())
		BackToMap();
	// If there are events to handle
	else
	{
		// Set the logic state so a sub event is selected
		logicState = LogicState::IN_EVENT;

		// Inform the user of the possible events to be picked
		// TODO: Introduce this text in the config file
		app->log("Choose an option:");
		for (vector<SubEvent>::iterator it = handlingEvent->subEvents.begin();
			it != handlingEvent->subEvents.end();
			it++)
		{
			app->log(("    -" + (*it).option).c_str());
		}
	}
}

ModuleGameLogic::LoadEventResult ModuleGameLogic::LoadEvent()
{
	LoadEventResult ret = LoadEventResult::FATAL_ERROR;

	// Try to load an event depending on the logic state
	switch (logicState)
	{
		// If the map is being navigated, update position
		case LogicState::NAVIGATING_MAP:
		{
			// If the moving command was invalid skip the logical frame
			if (!UpdateGridPosition())
			{
				handlingEvent = nullptr;
				ret = LoadEventResult::INVALID_INPUT;
				break;
			}
		}
		// If the map is being navigated OR we are in an initialization phase, load map event
		case LogicState::INITIALIZATION:
		{
			// Get map event
			MapEvent* currentMapEvent = GetCurrentMapEvent();


			// There is no current map event
			if (currentMapEvent == nullptr)
			{
				// Something went terribly wring
				app->log("FATAL ERROR: The grid position didn't had a matching map event");
				ret = LoadEventResult::FATAL_ERROR;
			}
			// The map event is not navigable
			else if (!currentMapEvent->navigable) 
			{
				app->log(currentMapEvent->text.c_str());
				gameState.currentGridPosition = previousGridPosition;
				ret = LoadEventResult::EVENT_ENDED;
			}
			// The map event is navigable
			else 
			{
				// TODO: Si se cumplen las condiciones de un evento alternativo se carga ese en su lugar
				handlingEvent = currentMapEvent;
				ret = LoadEventResult::EVENT_LOADED;
			}

			break;
		}
		// If there is an event to handle use the input to load a sub event
		case LogicState::IN_EVENT:
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
				bool conditionsMet = ContainerUtils::stringVectorAContainsB(choosenOption->conditions, gameConditionsVector);

				// If the conditions have been met
				if (conditionsMet)
				{
					// Load the sub event
					handlingEvent = choosenOption;
					ret = LoadEventResult::EVENT_LOADED;
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
						if (ContainerUtils::stringVectorAContainsB((*it).conditions, gameConditionsVector))
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
						app->log(rejectionText->text.c_str());
					}
					// The rejection text was not found
					else 
					{
						// Display default rejection text
						app->log(app->gameImporter->config.defaultSubEventRejectionMessage.c_str());
						handlingEvent = nullptr;
					}

					// Go back to navigate the map
					ret = LoadEventResult::EVENT_ENDED;
					BackToMap();
				}
			}
			// The choosen option was not avaliable
			else 
			{
				app->log("That is not an option");
				ret = LoadEventResult::INVALID_INPUT;
			}
			break;
		}

		case LogicState::SAVING_VARIABLE: 
		{
			// TODO: Develop system to save variables
		}
	}

	return ret;
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
		gameState.currentGridPosition += app->gameImporter->config.gridRowLength;
	else if (app->input->currentLoopInput == "south")
		gameState.currentGridPosition -= app->gameImporter->config.gridRowLength;
	else if (app->input->currentLoopInput == "east")
		gameState.currentGridPosition ++;
	else if (app->input->currentLoopInput == "west")
		gameState.currentGridPosition--;
	else 
	{
		ret = false;
		app->log("Invalid moving input, please introduce another command");
	}

	return ret;

}

void ModuleGameLogic::BackToMap()
{
	app->log("Where do you want to go?");
	logicState = LogicState::NAVIGATING_MAP;
}

void ModuleGameLogic::SaveVariable(string newVariableSaving)
{
	
}

void ModuleGameLogic::GameData::Save(JSON_Object* s_GameData)
{
	// TODO: Serialization of the game file
}


