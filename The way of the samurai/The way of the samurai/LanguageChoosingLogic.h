#pragma once
#include "LogicProcessor.h"
#include <vector>
#include <map>

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
	map<string, string>* savedVariables;

	// Parameters
	vector<string> languages;
	string invalidOptionText;
	string languageChoosingQuestionText;

	friend class ModuleGameLogic;
};

