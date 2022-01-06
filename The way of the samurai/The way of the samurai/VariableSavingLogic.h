#pragma once
#include "LogicProcessor.h"
#include <map>
#include <vector>

class Event;

enum class SavingVariableState 
{
	SAVING,
	CONFIRMING
};
class VariableSavingLogic : public LogicProcessor
{
private:
	// Methods (external)
	// Reads user input to make choices about the state of "savedVariables"
	LogicProcessorResult Step(string input) override;
	// Loads the data for saving the variable
	void LoadSavingVariable(Event* newSavingVariableEvent);

	// Methods (internal)
	// Saves the variable inputed for the player and triggers confirmation
	LogicProcessorResult SaveInputedVariable(string input);
	// Performs confirmation for the saved variable
	LogicProcessorResult ConfirmSavedVariable(string input);


	// References
	map<string, string>* savedVariables;
	void (*displayOptions)(const vector<string>&);

	// Parameters
	// Variable being saved when stepping
	Event* savingVariableEvent;

	// Variables
	SavingVariableState savingVariableState;

	friend class ModuleGameLogic;
};

