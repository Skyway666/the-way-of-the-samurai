#include "MapEventsLogic.h"
#include "ModuleGameImporter.h"
#include "ContainerUtils.h"

LogicProcessorResult MapEventsLogic::Step(string input)
{
	LogicProcessorResult ret = LogicProcessorResult::NONE;
	// Decide how to handle the current event depending on the state of it
	switch (handlingEventState) 
	{
		case HandlingEventState::BRANCHING:
		{
			ret = HandleCurrentEventBranching(input);
			break;
		}
		case HandlingEventState::SAVING_VARIABLE:
		{
			ret = HandleCurrentEventVariableSaved();
			break;
		}
	}

    return ret;
}

LogicProcessorResult MapEventsLogic::LoadMapEvent(MapEvent* mapEvent)
{
	LogicProcessorResult ret = LogicProcessorResult::NONE;
	// There is no current map event
	if (mapEvent == nullptr)
	{
		ret = LogicProcessorResult::MAP_EVENT_INVALID;
	}
	// The event can be loaded
	else
	{
		// Load map event
		SetHandlingEvent(mapEvent);

		// If the loaded event is not navigable
		if (!((MapEvent*)handlingEvent)->navigable)
		{
			// Display text and go back to previous position
			log(mapEvent->text);
			ret = LogicProcessorResult::MAP_EVENT_NOT_NAVIGABLE;
		}
		else 
		{
			// If it is navigable the event is handled
			ret = HandleCurrentEvent();
		}
	}

	return ret;
}

void MapEventsLogic::SetHandlingEvent(Event* newHandlingEvent)
{
	// Convert list of condition to a vector
	vector<string> gameConditionsVector = vector<string>(conditions->begin(), conditions->end());

	// Check if an alternative event should be loaded instead
	Event* alternativeEvent = nullptr;
	for (AlternativeEvent& currAlternativeEvent : newHandlingEvent->alternativeEvents)
	{
		// Check if the conditions are met
		if (ContainerUtils::stringVectorAContainsB(gameConditionsVector, currAlternativeEvent.conditions))
		{
			alternativeEvent = currAlternativeEvent.alternative;
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

LogicProcessorResult MapEventsLogic::HandleCurrentEvent()
{
	// Display event text
	log(handlingEvent->text);

	// Obtain conditions
	for (string& condition : handlingEvent->obtainedConditions)
	{
		// The condition is not in the list so it can be added
		if (find(conditions->begin(), conditions->end(), condition) == conditions->end())
			conditions->push_back(condition);
	}

	// Remove conditions
	for (string& removedCondition : handlingEvent->removedConditions)
		conditions->remove(removedCondition);

	// Obtain objects
	for (string& obtainedObject : handlingEvent->obtainedObjects)
		objects->push_back(obtainedObject);

	LogicProcessorResult ret = LogicProcessorResult::NONE;
	// There is a variable to save: Priority 1
	if (handlingEvent->savedVariable != nullptr)
	{
		ret = LogicProcessorResult::MAP_EVENT_SAVE_VARIABLE;
		handlingEventState = HandlingEventState::SAVING_VARIABLE;
	}
	// There are sub events to handle: Priority 2
	else if (!handlingEvent->subEvents.empty())
	{
		HandleSubEventsDisplay(handlingEvent);
		ret = LogicProcessorResult::MAP_EVENT_BRANCHING;
		handlingEventState = HandlingEventState::BRANCHING;
	}
	// There is nothing else to do
	else
	{
		// Go back to navigating the map
		ret = LogicProcessorResult::MAP_EVENT_ENDED;
	}

	return ret;
}

void MapEventsLogic::HandleSubEventsDisplay(Event* eventToBranch)
{
	// Save options for the possible sub events
	vector<string> options;
	for (SubEvent& subEvent : handlingEvent->subEvents)
	{
		options.push_back(subEvent.option);
	}

	// Display options
	displayOptions(options);
}

LogicProcessorResult MapEventsLogic::HandleCurrentEventBranching(string input)
{
	// Check if the provided input leads to a sub event
	SubEvent* choosenOption = nullptr;
	for (SubEvent& subEvent : handlingEvent->subEvents)
	{
		if (subEvent.option == input)
			choosenOption = &subEvent;
	}

	LogicProcessorResult ret = LogicProcessorResult::NONE;
	// An option was choosen
	if (choosenOption != nullptr)
	{
		// Convert list of condition to a vector
		vector<string> gameConditionsVector = vector<string>(conditions->begin(), conditions->end());
		// Check if the conditions for the sub event are met
		bool conditionsMet = ContainerUtils::stringVectorAContainsB(gameConditionsVector, choosenOption->conditions);

		// If the conditions have been met
		if (conditionsMet)
		{
			// Load the sub event
			SetHandlingEvent(choosenOption);
			// Handle sub event and save result
			ret = HandleCurrentEvent();
		}
		// The conditions have not been met
		else
		{
			// Load the rejection text based on the missing conditions
			RejectionText* rejectionText = nullptr;
			for (RejectionText& currRejectionText : choosenOption->rejectionTexts)
			{
				// If the conditions for the rejection text are met
				if (ContainerUtils::stringVectorAContainsB(gameConditionsVector, currRejectionText.conditions))
				{
					// Save the rejection text
					rejectionText = &currRejectionText;
					break;
				}
			}

			// The rejection text was found
			if (rejectionText != nullptr)
			{
				// Display the rejection text
				log(rejectionText->text);
			}
			// The rejection text was not found
			else
			{
				// Display default rejection text
				log(defaultSubEventRejectionText);
			}

			ret = LogicProcessorResult::MAP_EVENT_ENDED;
		}
	}
	// The choosen option was not avaliable
	else
	{
		log(invalidOptionText);
		ret =  LogicProcessorResult::MAP_EVENT_BRANCHING;
	}

	return ret;
}

LogicProcessorResult MapEventsLogic::HandleCurrentEventVariableSaved()
{
	LogicProcessorResult ret = LogicProcessorResult::NONE;
	// There is an event after saving variable
	if (handlingEvent->savedVariable->nextEvent != nullptr)
	{
		// Load next 
		SetHandlingEvent(handlingEvent->savedVariable->nextEvent);
		ret = HandleCurrentEvent();
	}
	// There is no next event
	else if (!handlingEvent->subEvents.empty())
	{
		// Handle the sub events
		HandleSubEventsDisplay(handlingEvent);
		ret = LogicProcessorResult::MAP_EVENT_BRANCHING;
	}
	// There is no next event nor sub events
	else
	{
		// The event has ended, so go back to map
		ret = LogicProcessorResult::MAP_EVENT_ENDED;
	}

	return ret;
}