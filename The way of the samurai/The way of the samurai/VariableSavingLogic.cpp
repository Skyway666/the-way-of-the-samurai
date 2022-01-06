#include "VariableSavingLogic.h"
#include "ModuleGameImporter.h"


LogicProcessorResult VariableSavingLogic::Step(string input)
{
	LogicProcessorResult ret = LogicProcessorResult::NONE;

	switch (savingVariableState)
	{
		case SavingVariableState::SAVING:
		{
			ret = SaveInputedVariable(input);
			break;
		}
		case SavingVariableState::CONFIRMING:
		{
			ret = ConfirmSavedVariable(input);
			break;
		}
	}

	return ret;
}

void VariableSavingLogic::LoadSavingVariable(Event* newSavingVariableEvent)
{
	savingVariableEvent = newSavingVariableEvent;
	savingVariableState = SavingVariableState::SAVING;
}

LogicProcessorResult VariableSavingLogic::SaveInputedVariable(string input)
{
	(*savedVariables)[savingVariableEvent->savedVariable->key.c_str()] = input;

	// Display confirmation text
	log(savingVariableEvent->savedVariable->confirmationText);

	// Display options
	vector<string> options = { "yes", "no" };
	displayOptions(options);

	// After saving the variable we need confirmation
	savingVariableState = SavingVariableState::CONFIRMING;
	return LogicProcessorResult::VARIABLE_SAVING_SAVING;
}

LogicProcessorResult VariableSavingLogic::ConfirmSavedVariable(string input)
{
	LogicProcessorResult ret = LogicProcessorResult::NONE;
	// User confirms the variable
	if (input == "yes")
	{
		// Display success to the user
		log(savingVariableEvent->savedVariable->successText);
		ret = LogicProcessorResult::VARIABLE_SAVING_SAVED;
	}
	// User doesn't confirm the variable
	else if (input == "no")
	{
		// Go back to the user writing the variable
		log(savingVariableEvent->text);
		savingVariableState = SavingVariableState::SAVING;
		ret = LogicProcessorResult::VARIABLE_SAVING_SAVING;
	}
	else
	{
		// Invalid input. TODO: Include in config
		log("Invalid input");
		ret = LogicProcessorResult::VARIABLE_SAVING_SAVING;
	}

	return ret;
}
