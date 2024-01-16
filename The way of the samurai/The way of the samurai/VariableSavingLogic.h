#pragma once
#include "LogicProcessor.h"
#include <map>
#include <vector>

struct Event;

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
	void LoadSavingVariableEvent(Event* newSavingVariableEvent);

	// Methods (internal)
	// Saves the variable inputed for the player and triggers confirmation
	LogicProcessorResult SaveInputedVariable(string input);
	// Performs confirmation for the saved variable
	LogicProcessorResult ConfirmSavedVariable(string input);


	// References
	map<string, string>* savedVariables;
	void (*displayOptions)(const vector<string>&);
	void (*processGameplayText)(string&);

	// Parameters
	// Variable being saved when stepping
	Event* savingVariableEvent;
	string invalidOptionText;
	string yesInputText;
	string noInputText;

	// Variables
	SavingVariableState savingVariableState;

	friend class ModuleGameLogic;
};

