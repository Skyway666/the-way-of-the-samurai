#pragma once
#include "LogicProcessor.h"
#include <vector>
class LanguageChoosingLogic : public LogicProcessor
{
private:
	// Methods (external)
	// Reads user input to make choices about the language being choosen
	LogicProcessorResult Step(string input) override;
	// Starts the language choosing process
	void StartLanguageChoosing();

	// References
	void (*displayOptions)(const vector<string>&);

	// Parameters
	vector<string> languages;

	// Variables
	string choosenLanguage;

	friend class ModuleGameLogic;
};

