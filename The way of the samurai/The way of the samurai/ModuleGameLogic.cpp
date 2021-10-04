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
	bool ret = true;

	// Loads the next event by reading input
	ret = LoadEvent();

	// Handle event
	if(handlingEvent != nullptr)
		HandleCurrentEvent();

	return ret;
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
		logicState = LogicState::NavigatingMap;
	else 
	{
		logicState = LogicState::InEvent;

		// TODO: Display possible sub events
	}


}

bool ModuleGameLogic::LoadEvent()
{
	switch (logicState) 
	{
		// If the map is being navigated, update position
		case LogicState::NavigatingMap:
			UpdateGridPosition();
		// If the map is being navigated OR we are in an initialization phase, load map event
		case LogicState::Initialization:
		{
			// Get map event
			handlingEvent = GetCurrentMapEvent();

			// If there is no current map event
			if (handlingEvent == nullptr)
			{
				// Something went terribly wring
				app->log("FATAL ERROR: The grid position didn't had a matching map event");
				return false;
			}
			break;
		}
		// If there is an event to handle use the input to load a sub event
		case LogicState::InEvent:
		{
			/* TODO: Load sub event
				- Check if the provided input leads to a sub event
				- Check if the conditions for the sub event are met
				- If they are, load the sub event
				- If they aren't, load the rejection text and return to the move state.
			*/
			break;
		}
	}

	return true;
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

void ModuleGameLogic::UpdateGridPosition()
{
	// TODO: : Read input and update grid position (north, south, east, west)

	if (app->input->currentLoopInput == "increase")
		gameState.currentGridPosition++;
	else if (app->input->currentLoopInput == "decrease")
		gameState.currentGridPosition--;
	else
		app->log("Invalid moving input, please introduce another command");
}

void ModuleGameLogic::GameData::Save(JSON_Object* s_GameData)
{
	// TODO: Serialization of the game file
}


