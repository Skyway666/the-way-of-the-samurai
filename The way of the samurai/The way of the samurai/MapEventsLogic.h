#pragma once
#include "LogicProcessor.h"
#include <list>
#include <vector>

class MapEvent;
class Event;

enum class HandlingEventState 
{
	NONE,
	BRANCHING,
	SAVING_VARIABLE
};

class MapEventsLogic : public LogicProcessor
{
private:
	// Methods (external)
	// Reads user input to make choices about event handling
	LogicProcessorResult Step(string input) override;
	// Initial Loading of a map event
	LogicProcessorResult LoadMapEvent(MapEvent* mapEvent);

	// Methods (internal)
	// Sets the new event to handle checking for alternative events
	void SetHandlingEvent(Event* newHandlingEvent);
	// Handles an event
	LogicProcessorResult HandleCurrentEvent();
	// Handles the display of sub events of an event
	void HandleSubEventsDisplay(Event* eventToBranch);
	// Handles sub events of the current event
	LogicProcessorResult HandleCurrentEventBranching(string input);
	// Ends event handling after a variable has been saved
	LogicProcessorResult HandleCurrentEventVariableSaved();

	// References
	list<string>* conditions;
	list<string>* objects;
	void (*displayOptions)(const vector<string>&);

	// Parameters
	string defaultSubEventRejectionMessage;
	// Event being handled.
	Event* handlingEvent = nullptr;

	// Variables
	// What to do with the handling event when "Step" is called
	HandlingEventState handlingEventState = HandlingEventState::NONE;

	friend class ModuleGameLogic;
};

