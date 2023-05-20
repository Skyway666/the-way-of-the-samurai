#pragma once
#include "LogicProcessor.h"
class MapNavigationLogic : public LogicProcessor
{
private:
	// Methods
	// Reads user input to make choices about the value of "currentGridPosition"
	LogicProcessorResult Step(string input) override;
	// Goes back to the previous grid position
	void BackToPreviousGridPosition();

	// References
	int* currentGridPosition = nullptr;

	// Parameters
	int gridRowLength = 0;
	string invalidOptionText;

	// Variables
	int previousGridPosition = 0;

	friend class ModuleGameLogic;
};

