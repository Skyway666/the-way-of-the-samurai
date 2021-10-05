#include "ModuleGameLogic.h"
#include "Third Party/parson.h"
#include "ModuleGameImporter.h"
#include "ModuleInput.h"
#include "Application.h"
#include <vector>

using namespace std;

bool ModuleGameLogic::Init()
{
	// TODO: Load game state from save file
	return true;
}

bool ModuleGameLogic::Start()
{
	/* TODO: If there is no save file:
		- Assign initial grid position from GameImporter
		- Display initial text
		- Ask player name if it is enabled
	*/

	app->log("Welcome to the most epic text adventure ever!");
	gameState.currentGridPosition = 1;
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

	// TODO: Inform the user about the obtained conditions

	// Obtain objects
	for (vector<string>::iterator it = handlingEvent->obtainedObjects.begin();
		it != handlingEvent->obtainedObjects.end();
		it++)
		gameState.objects.push_back((*it));

	// TODO: Indorm the user about the obtained objects

	// If there are no events to handle, go back to navigating the map
	if (handlingEvent->subEvents.empty())
		BackToMap();
	// If there are events to handle
	else
	{
		// Set the logic state so a sub event is selected
		logicState = LogicState::IN_EVENT;

		// Inform the user of the possible events to be picked
		app->log("Pick one of the possible sub events:");
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
			handlingEvent = GetCurrentMapEvent();

			// If there is no current map event
			if (handlingEvent == nullptr)
			{
				// Something went terribly wring
				app->log("FATAL ERROR: The grid position didn't had a matching map event");
				ret = LoadEventResult::FATAL_ERROR;
			}
			// If there is a map event
			else 
				ret = LoadEventResult::EVENT_LOADED;
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
				// Check if the conditions for the sub event are met
				bool conditionsMet = true;
				for (vector<string>::iterator it = choosenOption->conditions.begin();
					it != choosenOption->conditions.end();
					it++) 
				{
					// If the condition of the choosenOption is not within the gameState conditions
					if (find(gameState.conditions.begin(), gameState.conditions.end(), (*it)) 
						== 
						gameState.conditions.end()) 
					{
						// The conditions are not met
						conditionsMet = false;
						break;
					}
				}

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
					// Load the rejection text 
					// TODO: Choose the rejection text based on the missing conditions
					app->log("The conditions for picking this sub event are not met.");
					handlingEvent = nullptr;

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
	// TODO: : Read input and update grid position (north, south, east, west)

	bool ret = true;
	if (app->input->currentLoopInput == "increase")
		gameState.currentGridPosition++;
	else if (app->input->currentLoopInput == "decrease")
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

void ModuleGameLogic::GameData::Save(JSON_Object* s_GameData)
{
	// TODO: Serialization of the game file
}


