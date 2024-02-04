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
#include "Third Party/parson.h"

#include <iostream>

void ModuleGameLogic::GameState::Save(const char* path)
{
	// Initialize json object
	JSON_Value* rawGameData = json_value_init_object();
	JSON_Object* s_gameState = json_value_get_object(rawGameData);

	// Save conditions array
	JSON_Value* rawConditionsArray = json_value_init_array();
	json_object_set_value(s_gameState, "conditions", rawConditionsArray);
	for (string condition : conditions)
		json_array_append_string(json_value_get_array(rawConditionsArray), condition.c_str());

	// Save objects array
	JSON_Value* rawObjectsArray = json_value_init_array();
	json_object_set_value(s_gameState, "objects", rawObjectsArray);
	for (string object : objects)
		json_array_append_string(json_value_get_array(rawObjectsArray), object.c_str());

	// Save saved variables
	JSON_Value* rawSavedVariablesObject = json_value_init_object();
	json_object_set_value(s_gameState, "savedVariables", rawSavedVariablesObject);
	for (pair<string, string> savedVariable : savedVariables)
		json_object_set_string(json_value_get_object(rawSavedVariablesObject), 
			savedVariable.first.c_str(), savedVariable.second.c_str());

	// Save current grid position
	json_object_set_number(s_gameState, "currentGridPosition", currentGridPosition);

	// Save json object
	json_serialize_to_file_pretty(rawGameData, path);
	json_value_free(rawGameData);
}

void ModuleGameLogic::GameState::Load(const char* path)
{
	// Try to load file
	JSON_Value* rawGameData = json_parse_file(path);
	
	// Handle file not existing
	if (rawGameData == nullptr)
		return;

	// Retreive object
	JSON_Object* s_gameState = json_value_get_object(rawGameData);

	// Load conditions
	JSON_Array* s_conditions = json_object_get_array(s_gameState, "conditions");
	for (int i = 0; i < json_array_get_count(s_conditions); i++)
		conditions.push_back(json_array_get_string(s_conditions, i));

	// Load objects
	JSON_Array* s_objects = json_object_get_array(s_gameState, "objects");
	for (int i = 0; i < json_array_get_count(s_objects); i++)
		objects.push_back(json_array_get_string(s_objects, i));

	// Load savedVariables
	JSON_Object* s_savedVariables = json_object_get_object(s_gameState, "savedVariables");
	for (int i = 0; i < json_object_get_count(s_savedVariables); i++) 
	{
		// Load key and value
		const char* savedVariableKey = json_object_get_name(s_savedVariables, i);
		const char* savedVariableValue = json_object_get_string(s_savedVariables, savedVariableKey);

		// Save to map
		savedVariables[savedVariableKey] = savedVariableValue;
	}

	// Load grid position
	currentGridPosition = json_object_get_number(s_gameState, "currentGridPosition");

	json_value_free(rawGameData);
	loaded = true;
}

void ModuleGameLogic::Init()
{
	name = "Game Logic";
}

void ModuleGameLogic::Start()
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
	optionsChoosing->options.push_back(app->gameImporter->config->saveInputText);
	optionsChoosing->options.push_back(app->gameImporter->config->helpInputText);
	optionsChoosing->options.push_back(app->gameImporter->config->languageInputText);
	optionsChoosing->options.push_back(app->gameImporter->config->resumeInputText);
	optionsChoosing->options.push_back(app->gameImporter->config->exitInputText);

	optionsChoosing->optionsMenuIntroductionText = app->gameImporter->config->optionsMenuIntroductionText;
	optionsChoosing->availableObjectsText = app->gameImporter->config->availableObjectsText;
	optionsChoosing->currentConditionsText = app->gameImporter->config->currentConditionsText;
	optionsChoosing->savedText = app->gameImporter->config->savedText;
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

	optionsChoosing->saveGameState = SaveGameLogicGameState;

	// Load game state
	gameState.Load(gameStatePath);

	// Set initial position if no game was loaded
	if(!gameState.loaded)
		gameState.currentGridPosition = app->gameImporter->config->initialPosition;

	// Ask for language if the game is localized and there is no language set already
	if (app->localization->GetLanguages().size() != 0 && app->localization->GetLanguage() == "none")
	{
		app->localization->SetUserDefaultUILanguage();
		SetLogicProcessor(languageChoosing);
		languageChoosing->StartLanguageChoosing();
	}
	// A language is set or the game is non localized, start
	else
		StartPlaying();
}

void ModuleGameLogic::StartPlaying()
{
	// Log initial text if no game was loaded
	if(!gameState.loaded)
		LogGameplayText(app->gameImporter->config->initialText);

	// Load current map event
	SetLogicProcessor(mapEvents);
	HandleLogicProcessorResult(mapEvents->LoadMapEvent(GetCurrentMapEvent()));
}

void ModuleGameLogic::SetLogicProcessor(LogicProcessor* newLogicProcessor)
{
	lastLogicProcessor = currentLogicProcessor;
	currentLogicProcessor = newLogicProcessor;
}

void ModuleGameLogic::HandleLogicProcessorResult(LogicProcessorResult result)
{
	switch (result)
	{
		#pragma region MAP_NAVIGATION
		case LogicProcessorResult::MAP_NAVIGATION_INVALID:
			break;
		case LogicProcessorResult::MAP_NAVIGATION_SUCCESSFUL:
		{
			// Load event and handle the result recursively
			HandleLogicProcessorResult(mapEvents->LoadMapEvent(GetCurrentMapEvent()));
			break;
		}
		#pragma endregion
		#pragma region MAP_EVENT
		case LogicProcessorResult::MAP_EVENT_INVALID:
		{
			// Fatal error
			TerminateApplication("The grid position didn't had a matching map event");
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

			// Start playing if there was no previous logical state
			if (currentLogicProcessor == nullptr)
				StartPlaying();

			// If the previous logical state was options choosing, artificially start it
			if (currentLogicProcessor == optionsChoosing)
				optionsChoosing->StartOptionChoosing();
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
			LogGameplayText(app->gameImporter->config->exitGameText);
			app->Exit();
		}
		#pragma endregion
	}
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

void ModuleGameLogic::Update()
{
	// Let logic processor handle user input
	LogicProcessorResult result = currentLogicProcessor->Step(app->input->currentLoopInput);

	// React to the logic processor result
	HandleLogicProcessorResult(result);
}

void ModuleGameLogic::CleanUp()
{
	delete mapNavigation, mapEvents, variableSaving;
	mapNavigation =  nullptr;
	mapEvents = nullptr;
	variableSaving = nullptr;
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
			TerminateApplication("Variable without closing '@'");
			break;
		}

		// Get substring between @
		string variableKey = text.substr(openingAdd + 1, variableKeyLength - 1);

		// Check that the variable is in the dictionary
		if (gameState.savedVariables.find(variableKey) == gameState.savedVariables.end())
		{
			// Inform the user of the variable that wasn't found
			TerminateApplication(("Variable key: '" + variableKey + "' not found in variables dictionary").c_str());
			break;
		}

		// Finally, replace key with value
		text.replace(openingAdd, variableKeyLength + 1, gameState.savedVariables.at(variableKey));
	}
}

void ModuleGameLogic::SaveGameState()
{
	gameState.Save(gameStatePath);
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

void SaveGameLogicGameState()
{
	app->gameLogic->SaveGameState();
}