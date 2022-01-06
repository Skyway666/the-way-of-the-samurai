#pragma once
#include<string>

using namespace std;

class ModuleGameLogic;

enum class LogicProcessorResult
{
	NONE,
	// MAP NAVIGATION
	MAP_NAVIGATION_SUCCESSFUL,
	MAP_NAVIGATION_INVALID,
	// MAP EVENTS
	MAP_EVENT_INVALID,
	MAP_EVENT_NOT_NAVIGABLE,
	MAP_EVENT_BRANCHING,
	MAP_EVENT_SAVE_VARIABLE,
	MAP_EVENT_ENDED,
	// VARIABLE SAVING
	VARIABLE_SAVING_SAVING,
	VARIABLE_SAVING_SAVED,
	// LANGUAGE CHOOSING
	LANGUAGE_CHOOSING_CHOOSING,
	LANGUAGE_CHOOSING_CHOSEN
};

class LogicProcessor
{
protected:
	virtual LogicProcessorResult Step(string input) { return LogicProcessorResult::NONE; }
	void (*log)(string);


	friend class ModuleGameLogic;
};

