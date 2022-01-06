#include "ModuleGameLogic.h"
#include "Application.h"
#include "ModuleGameImporter.h"
#include "ModuleLocalization.h"
#include "ModuleInput.h"
#include "MapNavigationLogic.h"
#include "MapEventsLogic.h"
#include "VariableSavingLogic.h"
#include "LanguageChoosingLogic.h"

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

	// Intialize map events logic
	mapEvents = new MapEventsLogic();
	mapEvents->defaultSubEventRejectionMessage = app->gameImporter->config->defaultSubEventRejectionMessage;

	// Intialize variable saving logic
	variableSaving = new VariableSavingLogic();

	// Initialize languge choosing logic
	languageChoosing = new LanguageChoosingLogic();
	languageChoosing->languages = app->localization->GetLanguages();

	// Initialize all logic processor references
	mapNavigation->currentGridPosition = &gameState.currentGridPosition;

	mapEvents->conditions = &gameState.conditions;
	mapEvents->objects = &gameState.objects;
	variableSaving->savedVariables = &gameState.savedVariables;
	mapNavigation->log = 
		mapEvents->log = 
		variableSaving->log = 
		languageChoosing->log =  LogGameplayText;
	mapEvents->displayOptions = 
		variableSaving->displayOptions =
		languageChoosing->displayOptions =
		DisplayOptions;

	// Initialize position with initial position
	gameState.currentGridPosition = app->gameImporter->config->initialPosition;

	// Log initial text
	LogGameplayText(app->gameImporter->config->initialText);

	// There is no language set
	if (app->localization->GetLanguage() == "none")
	{
		SetLogicProcessor(languageChoosing);
		languageChoosing->StartLanguageChoosing();
	}
	// A language is set, load the even in the first grid position
	else
	{
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
		case LogicProcessorResult::MAP_NAVIGATION_INVALID:
			break;
		case LogicProcessorResult::MAP_NAVIGATION_SUCCESSFUL:
		{
			// Load event and handle the result recursively
			ret = HandleLogicProcessorResult(mapEvents->LoadMapEvent(GetCurrentMapEvent()));
			break;
		}
		case LogicProcessorResult::MAP_EVENT_INVALID:
		{
			// Fatal error
			app->logFatalError("The grid position didn't had a matching map event");
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
			// Load saving variable and switch logic processor
			variableSaving->LoadSavingVariable(mapEvents->handlingEvent);
			SetLogicProcessor(variableSaving);
			break;
		}
		case LogicProcessorResult::VARIABLE_SAVING_SAVING:
			break;
		case LogicProcessorResult::VARIABLE_SAVING_SAVED:
		{
			// Go back to the last logical state
			BackToLastLogicalState(); 

			// If it is a map event, artificially step it
			if (currentLogicProcessor == mapEvents)
			{
				HandleLogicProcessorResult(mapEvents->Step(""));
			}
			break;
		}
		case LogicProcessorResult::LANGUAGE_CHOOSING_CHOOSING:
			break;
		case LogicProcessorResult::LANGUAGE_CHOOSING_CHOSEN:
		{
			// Set the choosen language
			app->localization->SetLanguage(languageChoosing->choosenLanguage);

			// Go back to the last logical state
			BackToLastLogicalState();

			// If there was no previous logical state
			if (currentLogicProcessor == nullptr)
			{
				// Load the event in the first grid position
				SetLogicProcessor(mapEvents);
				HandleLogicProcessorResult(mapEvents->LoadMapEvent(GetCurrentMapEvent()));
			}
			break;
		}
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
	// TODO: Include in config file
	LogGameplayText("Where do you want to go?");
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

void HandleLocalization(string& text)
{
	// 'text' exists as a localization entry key
	if (app->localization->Exists(text))
	{
		// Overrwrite 'text' with localization value
		text = app->localization->GetLocalizatedText(text);
	}
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
			app->logFatalError("Variable without closing '@'");
			break;
		}

		// Get substring between @
		string variableKey = text.substr(openingAdd + 1, variableKeyLength - 1);

		// Check that the variable is in the dictionary
		if (gameState.savedVariables.find(variableKey) == gameState.savedVariables.end())
		{
			// Inform the user of the variable that wasn't found
			app->logFatalError(("Variable key: '" + variableKey + "' not found in variables dictionary").c_str());
			break;
		}

		// Finally, replace key with value
		text.replace(openingAdd, variableKeyLength + 1, gameState.savedVariables.at(variableKey));
	}
}

void LogGameplayText(string text)
{
	// Handle localization
	HandleLocalization(text);

	// Add gameplay variables
	app->gameLogic->ReplaceVariables(text);

	// Display final text to the user
	app->log(text.c_str());
}

void DisplayOptions(const vector<string>& options)
{
	// TODO: Introduce this text in the config file
	LogGameplayText("Choose an option:");

	// Display options
	for (string option : options)
		app->log(("    -" + option).c_str());
}
