#include "LanguageChoosingLogic.h"

LogicProcessorResult LanguageChoosingLogic::Step(string input)
{
	LogicProcessorResult ret = LogicProcessorResult::NONE;

	// The provided input is a selectable language
	if (std::find(languages.begin(), languages.end(), input) != languages.end())
	{
		// Invalid input. TODO: Include in config
		log("Language '" + input + "' selected");
		// Set language
		choosenLanguage = input;
		// Communicate a language has been choosen
		ret = LogicProcessorResult::LANGUAGE_CHOOSING_CHOSEN;
	}
	// Invalid input not empty
	else 
	{
		// Invalid input. TODO: Include in config
		log("Invalid input");
		ret = LogicProcessorResult::LANGUAGE_CHOOSING_CHOOSING;
	}

	return ret;
}

void LanguageChoosingLogic::StartLanguageChoosing()
{
	// TODO: Check system language to ask this question in the appropiate language
	log("What language do you wish to play in? These are the avaliable languages:");

	// Display avaliable languages
	displayOptions(languages);
}