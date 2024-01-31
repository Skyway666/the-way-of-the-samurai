#include "ModuleGameLogic.h"
#include "Application.h"
#include "ModuleGameImporter.h"
#include "ModuleLocalization.h"
#include "ModuleInput.h"
#include "MapNavigationLogic.h"
#include "MapEventsLogic.h"
#include "VariableSavingLogic.h"
#include "LanguageChoosingLogic.h"
#include "OptionsChoosingLogic.h"

#include <iostream>

bool ModuleGameLogic::Init()
{
	name = "Game Logic";
	
	return true;
}

bool ModuleGameLogic::Start()
{
	// Initialize map navigation logic
	mapNavigation = new MapNavigationLogic();
	mapNavigation->gridRowLength = app->gameImporter->config->gridRowLength;
	mapNavigation->northInputText = app->gameImporter->config->northInputText;
	mapNavigation->southInputText = app->gameImporter->config->southInputText;
	mapNavigation->eastInputText = app->gameImporter->config->eastInputText;
	mapNavigation->westInputText = app->gameImporter->config->westInputText;
	mapNavigation->optionsInputText = app->gameImporter->config->optionsInputText;
	mapNavigation->reenterInputText = app->gameImporter->config->reenterInputText;

	// Intialize map events logic
	mapEvents = new MapEventsLogic();
	mapEvents->defaultSubEventRejectionText = app->gameImporter->config->defaultSubEventRejectionText;

	// Intialize variable saving logic
	variableSaving = new VariableSavingLogic();
	variableSaving->yesInputText = app->gameImporter->config->yesInputText;
	variableSaving->noInputText = app->gameImporter->config->noInputText;

	// Initialize languge choosing logic
	languageChoosing = new LanguageChoosingLogic();
	languageChoosing->languages = app->localization->GetLanguages();
	languageChoosing->languageChoosingQuestionText = app->gameImporter->config->languageChoosingQuestionText;

	// Initialize option choosing logic
	optionsChoosing = new OptionsChoosingLogic();
	optionsChoosing->options.push_back(app->gameImporter->config->objectsInputText);
	optionsChoosing->options.push_back(app->gameImporter->config->conditionsInputText);
	optionsChoosing->options.push_back(app->gameImporter->config->helpInputText);
	optionsChoosing->options.push_back(app->gameImporter->config->languageInputText);
	optionsChoosing->options.push_back(app->gameImporter->config->resumeInputText);
	optionsChoosing->options.push_back(app->gameImporter->config->exitInputText);

	optionsChoosing->optionsMenuIntroductionText = app->gameImporter->config->optionsMenuIntroductionText;
	optionsChoosing->availableObjectsText = app->gameImporter->config->availableObjectsText;
	optionsChoosing->currentConditionsText = app->gameImporter->config->currentConditionsText;
	optionsChoosing->helpText = app->gameImporter->config->helpText;

	// Initialize commun logic processor parameters
	mapNavigation->invalidOptionText = 
		mapEvents->invalidOptionText =
		variableSaving->invalidOptionText =
		languageChoosing->invalidOptionText = 
		optionsChoosing->invalidOptionText = app->gameImporter->config->invalidOptionText;
	
	// Initialize commun logic processor references
	mapNavigation->currentGridPosition = &gameState.currentGridPosition;

	mapEvents->conditions = 
		optionsChoosing->conditions = &gameState.conditions;

	mapEvents->objects =
		optionsChoosing->objects = &gameState.objects;
			
	variableSaving->savedVariables = 
	languageChoosing->savedVariables = 
		&gameState.savedVariables;

	mapNavigation->log = 
		mapEvents->log = 
		variableSaving->log = 
		languageChoosing->log =
		optionsChoosing->log = LogGameplayText;

	mapEvents->displayOptions = 
		variableSaving->displayOptions =
		languageChoosing->displayOptions = DisplayOptions;

	optionsChoosing->displayList = DisplayList;

	mapNavigation->processGameplayText = 
		variableSaving->processGameplayText = 
		optionsChoosing->processGameplayText = ProcessGameplayText;

	// Initialize position with initial position
	gameState.currentGridPosition = app->gameImporter->config->initialPosition;

	// There is no language set
	if (app->localization->GetLanguage() == "none")
	{
		app->localization->SetUserDefaultUILanguage();
		SetLogicProcessor(languageChoosing);
		languageChoosing->StartLanguageChoosing();
	}
	// A language is set, start the game
	else
	{
		// Log initial text
		LogGameplayText(app->gameImporter->config->initialText);

		// Load current map event
		SetLogicProcessor(mapEvents);
		HandleLogicProcessorResult(mapEvents->LoadMapEvent(GetCurrentMapEvent()));
	}


	return true;
}

void ModuleGameLogic::SetLogicProcessor(LogicProcessor* newLogicProcessor)
{
	lastLogicProcessor = currentLogicProcessor;
	currentLogicProcessor = newLogicProcessor;
}

bool ModuleGameLogic::HandleLogicProcessorResult(LogicProcessorResult result)
{
	bool ret = true;
	switch (result)
	{
		#pragma region MAP_NAVIGATION
		case LogicProcessorResult::MAP_NAVIGATION_INVALID:
			break;
		case LogicProcessorResult::MAP_NAVIGATION_SUCCESSFUL:
		{
			// Load event and handle the result recursively
			ret = HandleLogicProcessorResult(mapEvents->LoadMapEvent(GetCurrentMapEvent()));
			break;
		}
		#pragma endregion
		#pragma region MAP_EVENT
		case LogicProcessorResult::MAP_EVENT_INVALID:
		{
			// Fatal error
			app->LogFatalError("The grid position didn't had a matching map event");
			ret = false;
			break;
		}
		case LogicProcessorResult::MAP_EVENT_NOT_NAVIGABLE:
		{
			// Go to previous grid and back to map navigation
			mapNavigation->BackToPreviousGridPosition();
			BackToMap();
			break;
		}
		case LogicProcessorResult::MAP_EVENT_ENDED:
		{
			// Go back to map navigation
			BackToMap();
			break;
		}
		case LogicProcessorResult::MAP_EVENT_BRANCHING:
		{
			SetLogicProcessor(mapEvents);
			break;
		}
		case LogicProcessorResult::MAP_EVENT_SAVE_VARIABLE:
		{
			// Switch logic processor and load saving variable event
			SetLogicProcessor(variableSaving);
			variableSaving->LoadSavingVariableEvent(mapEvents->handlingEvent);
			break;
		}
		#pragma endregion
		#pragma region VARIABLE_SAVING
		case LogicProcessorResult::VARIABLE_SAVING_SAVING:
			break;
		case LogicProcessorResult::VARIABLE_SAVING_SAVED:
		{
			// Go back to the last logical state
			BackToLastLogicalState();

			// If it is a map event, artificially step it
			if (currentLogicProcessor == mapEvents)
				HandleLogicProcessorResult(mapEvents->Step(""));
			break;
		}
		#pragma endregion
		#pragma region LANGUAGE_CHOOSING
		case LogicProcessorResult::LANGUAGE_CHOOSING_CHOOSING:
			break;
		case LogicProcessorResult::LANGUAGE_CHOOSING_CHOSEN:
		{
			// Set the choosen language
			app->localization->SetLanguage(gameState.savedVariables["language"]);

			// Inform the user of the selected language
			LogGameplayText(app->gameImporter->config->languageSelectedText);

			// Go back to the last logical state
			BackToLastLogicalState();

			// If there was no previous logical state
			if (currentLogicProcessor == nullptr)
			{
				// Log initial text
				LogGameplayText(app->gameImporter->config->initialText);

				// Load current map event
				SetLogicProcessor(mapEvents);
				HandleLogicProcessorResult(mapEvents->LoadMapEvent(GetCurrentMapEvent()));
			}

			// If the previous logical state was options choosing, artificially start it
			if (currentLogicProcessor == optionsChoosing)
			{
				optionsChoosing->StartOptionChoosing();
			}
			break;
		}
		#pragma endregion
		#pragma region OPTIONS_CHOOSING
		case LogicProcessorResult::OPTIONS_CHOOSING_START:
		{
			SetLogicProcessor(optionsChoosing);
			optionsChoosing->StartOptionChoosing();
			break;
		}
		case LogicProcessorResult::OPTIONS_CHOOSING_CHOOSING:
		{
			optionsChoosing->StartOptionChoosing();
			break;
		}
		case LogicProcessorResult::OPTIONS_CHOOSING_CHOOSE_LANGUAGE:
		{
			SetLogicProcessor(languageChoosing);
			languageChoosing->StartLanguageChoosing();
			break;
		}
		case LogicProcessorResult::OPTIONS_CHOOSING_RESUME:
		{
			BackToMap();
			break;
		}
		case LogicProcessorResult::OPTIONS_CHOOSING_EXIT:
		{
			ret = false;
			LogGameplayText(app->gameImporter->config->exitGameText);
		}
		#pragma endregion
	}

	return ret;
}

MapEvent* ModuleGameLogic::GetCurrentMapEvent() const
{
	// Iterate map events
	for (MapEvent& mapEvent : app->gameImporter->mapEvents)
	{
		// Return pointer to map event with the current grid position
		if (mapEvent.gridPosition == gameState.currentGridPosition)
			return &mapEvent;
	}

	// No map event corresponds to this grid position
	return nullptr;
}

void ModuleGameLogic::BackToMap()
{
	LogGameplayText(app->gameImporter->config->backToMapText);
	SetLogicProcessor(mapNavigation);
}

void ModuleGameLogic::BackToLastLogicalState()
{
	currentLogicProcessor = lastLogicProcessor;
}

bool ModuleGameLogic::Update()
{
	// Let logic processor handle user input
	LogicProcessorResult result = currentLogicProcessor->Step(app->input->currentLoopInput);

	// React to the logic processor result
	return HandleLogicProcessorResult(result);
}

bool ModuleGameLogic::CleanUp()
{
	delete mapNavigation, mapEvents, variableSaving;
	mapNavigation =  nullptr;
	mapEvents = nullptr;
	variableSaving = nullptr;

	return true;
}

void ModuleGameLogic::ReplaceVariables(string& text) const
{
	// Iterate until all '@' are substituted
	while (text.find('@') != string::npos)
	{
		// Get position of the first and second @
		int openingAdd = text.find('@', 0);
		int closingAdd = text.find('@', openingAdd + 1);

		// Compute length of the variable key plus both @ at the beggining and the end
		int variableKeyLength = closingAdd - openingAdd;

		// Error handling
		if (closingAdd == string::npos)
		{
			app->LogFatalError("Variable without closing '@'");
			break;
		}

		// Get substring between @
		string variableKey = text.substr(openingAdd + 1, variableKeyLength - 1);

		// Check that the variable is in the dictionary
		if (gameState.savedVariables.find(variableKey) == gameState.savedVariables.end())
		{
			// Inform the user of the variable that wasn't found
			app->LogFatalError(("Variable key: '" + variableKey + "' not found in variables dictionary").c_str());
			break;
		}

		// Finally, replace key with value
		text.replace(openingAdd, variableKeyLength + 1, gameState.savedVariables.at(variableKey));
	}
}

void ProcessGameplayText(string& text) 
{
	// Handle localization
	app->localization->HandleLocalization(text);

	// Add gameplay variables
	app->gameLogic->ReplaceVariables(text);
}

void LogGameplayText(string text)
{
	// Handle localization
	ProcessGameplayText(text);

	// Display final text to the user
	app->Log(text.c_str());
}

void DisplayOptions(const vector<string>& options)
{
	LogGameplayText(app->gameImporter->config->displayOptionsText);

	// Display options
	DisplayList(options);
}

void DisplayList(const vector<string>& list)
{
	for (string element : list) 
	{
		ProcessGameplayText(element);
		app->Log(("    - " + element).c_str());
	}
}
