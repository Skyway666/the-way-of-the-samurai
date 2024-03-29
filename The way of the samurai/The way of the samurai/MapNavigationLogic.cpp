#include "MapNavigationLogic.h"

LogicProcessorResult MapNavigationLogic::Step(string input)
{
	// Save previous position, in case we find a non navigatible tile
	previousGridPosition = *currentGridPosition;

	
	// Process options
	string north = northInputText;
	string south = southInputText;
	string east = eastInputText;
	string west = westInputText;
	string options = optionsInputText;
	string reenter = reenterInputText;
	processGameplayText(north);
	processGameplayText(south);
	processGameplayText(east);
	processGameplayText(west);
	processGameplayText(options);
	processGameplayText(reenter);

	// Grid movement
	LogicProcessorResult ret = LogicProcessorResult::MAP_NAVIGATION_SUCCESSFUL;
	if (input == north)
		*currentGridPosition += gridRowLength;
	else if (input == south)
		*currentGridPosition -= gridRowLength;
	else if (input == east)
		(*currentGridPosition)++;
	else if (input == west)
		(*currentGridPosition)--;
	else if (input == options)
		ret = LogicProcessorResult::OPTIONS_CHOOSING_START;
	// If the input is "reenter" we reproduce the same event again
	else if(input != reenter)
	{
		ret = LogicProcessorResult::MAP_NAVIGATION_INVALID;
		log(invalidOptionText);
	}

	return ret;
}

void MapNavigationLogic::BackToPreviousGridPosition()
{
    *currentGridPosition = previousGridPosition;
}