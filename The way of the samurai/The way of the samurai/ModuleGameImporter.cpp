#include "ModuleGameImporter.h"
#include "Application.h"
#include "Third Party/parson.h"

// Static variables declaration
vector<JSONFieldData> Config::mandatoryFields;
vector<JSONFieldData> Event::mandatoryFields;
vector<JSONFieldData> MapEvent::mandatoryFields;
vector<JSONFieldData> SubEvent::mandatoryFields;
vector<JSONFieldData> AlternativeEvent::mandatoryFields;
vector<JSONFieldData> RejectionText::mandatoryFields;
vector<JSONFieldData> SavedVariable::mandatoryFields;
vector<JSON_Value*> ModuleGameImporter::linkedFiles;

void ModuleGameImporter::Init()
{
	// Module name
	name = "Game Importer";

	// Set mandatory fields for every object
	InitMandatoryFields();

	// TODO: Look for a dynamic way to configure the name of the file being read as the game
	// Read file
	JSON_Value* rawFile = json_parse_file_with_comments("Game.json");
	// Error handling for missing file
	if (rawFile == nullptr) 
	{
		TerminateApplication("Couldn't load game file: 'Game.json'");
		return;
	}

	// Read root object
	JSON_Object* game = json_value_get_object(rawFile);
	// Error handling for missing root object in file
	if (game == nullptr) 
	{
		TerminateApplication("No root object in the game file");
		return;
	}

	// Read all map events
	JSON_Array* s_mapEvents = GetLinkableArray(json_object_get_value(game, "mapEvents"), "mapEvents");
	// Error handling for map events
	if (s_mapEvents == nullptr) 
	{
		TerminateApplication("No 'mapEvents' array in root object");
		return;
	}
	// Parse each map event
	for (int i = 0; i < json_array_get_count(s_mapEvents); i++) 
	{
		// Get object from array
		JSON_Object* s_mapEvent = GetLinkableObject(json_array_get_value(s_mapEvents, i), "mapEvents[x]");

		// Error handling
		if (s_mapEvent == nullptr) 
		{
			TerminateApplication(("No object in mapEvents[" + to_string(i) + "]").c_str());
			return;
		}

		// Load map event
		mapEvents.push_back(MapEvent(s_mapEvent));
	}

	// Read config
	JSON_Object* s_config = GetLinkableObject(json_object_get_value(game, "config"), "config");
	// Error handling for config file
	if(s_config == nullptr)
	{
		TerminateApplication("Couldn't load config file, terminating aplication");
		return;
	}
	// Parse config file
	config = new Config(s_config);

	// Clean game file
	json_value_free(rawFile);
	// Clean linked files
	for (JSON_Value* jsonValue : linkedFiles)
		json_value_free(jsonValue);
}

void ModuleGameImporter::CleanUp()
{
	// Clean up mandatory fields
	CleanUpMandatoryFields();

	// Clean config
	delete config;
	config = nullptr;

	// Clean map events
	for (MapEvent& mapEvent : mapEvents) 
	{
		// Clean saved variable in map event
		if (mapEvent.savedVariable != nullptr)
		{
			// Clean nextEvent in saved variable
			if (mapEvent.savedVariable->nextEvent != nullptr)
				delete mapEvent.savedVariable->nextEvent;

			delete mapEvent.savedVariable;
		}

		// Clean alternative events
		for (AlternativeEvent& alternativeEvent : mapEvent.alternativeEvents)
		{
			// Clean alternative in alternativeEvents
			if (alternativeEvent.alternative != nullptr)
				delete alternativeEvent.alternative;
		}

		mapEvent.alternativeEvents.clear();
	}

	mapEvents.clear();
	mapEvents.shrink_to_fit();
}

bool ModuleGameImporter::HandleMandatoryFields(JSON_Object* jsonObject, const char* objectType)
{
	// Get referece to the mandatory fields depending on the object type
	vector<JSONFieldData>* mandatoryFields = nullptr;

	if (objectType == "Config")
		mandatoryFields = &Config::mandatoryFields;
	else if (objectType == "Event")
		mandatoryFields = &Event::mandatoryFields;
	else if (objectType == "MapEvent")
		mandatoryFields = &MapEvent::mandatoryFields;
	else if (objectType == "SubEvent")
		mandatoryFields = &SubEvent::mandatoryFields;
	else if (objectType == "AlternativeEvent")
		mandatoryFields = &AlternativeEvent::mandatoryFields;
	else if (objectType == "RejectionText")
		mandatoryFields = &RejectionText::mandatoryFields;
	else if (objectType == "SavedVariable")
		mandatoryFields = &SavedVariable::mandatoryFields;

	// Check if the jsonObject has all the mandatory fields
	JSONFieldData missingField;
	if(mandatoryFields)
	 missingField = HasFields(jsonObject, *mandatoryFields);

	// Return true if no missing field was found and false otherwise
	bool ret = missingField.type == JSONNull;

	// If the object is missing a field
	if (!ret) 
	{
		// Inform the user about the missing mandatory field
		string errorLog = ("Object of type '" + string(objectType) + "' is missing the mandatory field '" + missingField.name + "'");
		app->Terminate(errorLog.c_str(), app->gameImporter);
	}

	return ret;
}

JSONFieldData ModuleGameImporter::HasFields(JSON_Object* jsonObject, vector<JSONFieldData>& fields)
{
	// Iterate mandatory fields
	for (JSONFieldData& field : fields)
		// If the field doesn't exist or doesn't have the correct type
		if (json_object_has_value_of_type(jsonObject, field.name.c_str(), field.type) == 0)
			// Return the missing mandatory field
			return field;

	// No mandatory field was missing
	return {};
}

void ModuleGameImporter::InitMandatoryFields()
{
	// Config
	Config::mandatoryFields.push_back({ "gridRowLength", JSONNumber });
	Config::mandatoryFields.push_back({ "initialPosition", JSONNumber });
	Config::mandatoryFields.push_back({ "initialText", JSONString});
	Config::mandatoryFields.push_back({ "defaultSubEventRejectionText", JSONString });
	Config::mandatoryFields.push_back({ "displayOptionsText", JSONString });
	Config::mandatoryFields.push_back({ "backToMapText", JSONString });
	Config::mandatoryFields.push_back({ "invalidOptionText", JSONString });
	Config::mandatoryFields.push_back({ "languageSelectedText", JSONString });
	Config::mandatoryFields.push_back({ "optionsMenuIntroductionText", JSONString });
	Config::mandatoryFields.push_back({ "availableObjectsText", JSONString });
	Config::mandatoryFields.push_back({ "currentConditionsText", JSONString });
	Config::mandatoryFields.push_back({ "exitGameText", JSONString });
	Config::mandatoryFields.push_back({ "helpText", JSONString });
	Config::mandatoryFields.push_back({ "yesInputText", JSONString });
	Config::mandatoryFields.push_back({ "noInputText", JSONString });
	Config::mandatoryFields.push_back({ "northInputText", JSONString });
	Config::mandatoryFields.push_back({ "southInputText", JSONString });
	Config::mandatoryFields.push_back({ "eastInputText", JSONString });
	Config::mandatoryFields.push_back({ "westInputText", JSONString });
	Config::mandatoryFields.push_back({ "optionsInputText", JSONString });
	Config::mandatoryFields.push_back({ "reenterInputText", JSONString });
	Config::mandatoryFields.push_back({ "objectsInputText", JSONString });
	Config::mandatoryFields.push_back({ "conditionsInputText", JSONString });
	Config::mandatoryFields.push_back({ "helpInputText", JSONString });
	Config::mandatoryFields.push_back({ "languageInputText", JSONString });
	Config::mandatoryFields.push_back({ "resumeInputText", JSONString });
	Config::mandatoryFields.push_back({ "exitInputText", JSONString });

	// Event
	Event::mandatoryFields.push_back({ "text" , JSONString});

	// SubEvent
	SubEvent::mandatoryFields.push_back({ "option", JSONString });

	// Rejection text
	RejectionText::mandatoryFields.push_back({ "text", JSONString });

	// Alternative event
	AlternativeEvent::mandatoryFields.push_back({ "conditions", JSONArray });
	AlternativeEvent::mandatoryFields.push_back({ "alternative", JSONObject });

	// Saved variable
	SavedVariable::mandatoryFields.push_back({ "key", JSONString });
	SavedVariable::mandatoryFields.push_back({ "confirmationText", JSONString });
	SavedVariable::mandatoryFields.push_back({ "successText", JSONString });
}

void ModuleGameImporter::CleanUpMandatoryFields()
{
	Config::mandatoryFields.clear();
	Event::mandatoryFields.clear();
	SubEvent::mandatoryFields.clear();
	RejectionText::mandatoryFields.clear();
	AlternativeEvent::mandatoryFields.clear();
	SavedVariable::mandatoryFields.clear();
}

JSON_Object* ModuleGameImporter::GetLinkableObject(JSON_Value* linkeableValue, const char* objectName)
{
	// Process
	JSON_Value* previousValue = linkeableValue;
	ProcessLinkableValue(linkeableValue, objectName, json_value_type::JSONObject);

	// Error handling
	JSON_Object* ret = json_value_get_object(linkeableValue);
	if (ret == nullptr && previousValue != linkeableValue)
	{
		// Inform the user about the error
		string errorLog = ("File for linked array '" + string(objectName) + "' didn't contain an object as root value");
		app->Terminate(errorLog.c_str(), app->gameImporter);
	}

	return ret;
}

JSON_Array* ModuleGameImporter::GetLinkableArray(JSON_Value* linkeableValue, const char* arrayName)
{
	// Process
	JSON_Value* previousValue = linkeableValue;
	ProcessLinkableValue(linkeableValue, arrayName, json_value_type::JSONArray);

	// Error handling
	JSON_Array* ret = json_value_get_array(linkeableValue);
	if (ret == nullptr && previousValue != linkeableValue)
	{
		// Inform the user about the error
		string errorLog = ("File for linked array '" + string(arrayName) + "' didn't contain an object as root value");
		app->Terminate(errorLog.c_str(), app->gameImporter);
	}

	return ret;
}

void ModuleGameImporter::ProcessLinkableValue(JSON_Value*& linkeableValue, const char* valueName, JSON_Value_Type valueType)
{
	// The "linkeableValue" contains a link to the file that contains the value
	if (json_value_get_type(linkeableValue) == json_value_type::JSONString)
	{
		// Load linked file
		const char* linkedArrayPath = json_value_get_string(linkeableValue);
		JSON_Value* rawValueFile = json_parse_file_with_comments(linkedArrayPath);
		linkeableValue = rawValueFile;
		// Error handling
		if (linkeableValue == nullptr)
		{
			// Inform the user about the error
			string errorLog = ("File for linked value '" + string(valueName) + "' with path '" + linkedArrayPath + "' could not be loaded");
			app->Terminate(errorLog.c_str(), app->gameImporter);
			return;
		}
		// Add loaded files for later cleaning
		ModuleGameImporter::linkedFiles.push_back(rawValueFile);
	}
}

JSONFieldData::JSONFieldData(string name, JSON_Value_Type type)
{
	this->name = name;
	this->type = type;
}

Config::Config(JSON_Object* s_config)
{
	// Check for mandatory fields
	if (!ModuleGameImporter::HandleMandatoryFields(s_config, "Config"))
		return;

	// Read gridRowLength
	gridRowLength = json_object_get_number(s_config, "gridRowLength");

	// Read initialPosition
	initialPosition = json_object_get_number(s_config, "initialPosition");

	// Read initialText
	initialText = json_object_get_string(s_config, "initialText");

	// Read defaultSubEventRejectionText
	defaultSubEventRejectionText = json_object_get_string(s_config, "defaultSubEventRejectionText");

	// Read displayOptionsText
	displayOptionsText = json_object_get_string(s_config, "displayOptionsText");

	// Read backToMapText
	backToMapText = json_object_get_string(s_config, "backToMapText");

	// Read invalidOptionText
	invalidOptionText = json_object_get_string(s_config, "invalidOptionText");

	// Read invalidCharacterText
	invalidCharacterText = json_object_get_string(s_config, "invalidCharacterText");

	// Read languageSelectedText
	languageSelectedText = json_object_get_string(s_config, "languageSelectedText");

	// Read avaliableObjectsText
	optionsMenuIntroductionText = json_object_get_string(s_config, "optionsMenuIntroductionText");

	// Read avaliableObjectsText
	availableObjectsText = json_object_get_string(s_config, "availableObjectsText");

	// Read currentConditionsText
	currentConditionsText = json_object_get_string(s_config, "currentConditionsText");

	// Read exitGameText
	exitGameText = json_object_get_string(s_config, "exitGameText");

	// Read helpText
	helpText = json_object_get_string(s_config, "helpText");

	// Read languageChoosingQuestionText
	languageChoosingQuestionText = json_object_get_string(s_config, "languageChoosingQuestionText");

	// Read yesInputText
	yesInputText = json_object_get_string(s_config, "yesInputText");

	// Read noInputText
	noInputText = json_object_get_string(s_config, "noInputText");

	// Read northInputText
	northInputText = json_object_get_string(s_config, "northInputText");

	// Read southInputText
	southInputText = json_object_get_string(s_config, "southInputText");

	// Read eastInputText
	eastInputText = json_object_get_string(s_config, "eastInputText");

	// Read westInputText
	westInputText = json_object_get_string(s_config, "westInputText");

	// Read reenterInputText
	optionsInputText = json_object_get_string(s_config, "optionsInputText");

	// Read reenterInputText
	reenterInputText = json_object_get_string(s_config, "reenterInputText");

	// Read objectsInputText
	objectsInputText = json_object_get_string(s_config, "objectsInputText");

	// Read conditionsInputText
	conditionsInputText = json_object_get_string(s_config, "conditionsInputText");

	// Read helpInputText
	helpInputText = json_object_get_string(s_config, "helpInputText");

	// Read languageInputText
	languageInputText = json_object_get_string(s_config, "languageInputText");

	// Read resumeInputText
	resumeInputText = json_object_get_string(s_config, "resumeInputText");

	// Read exitInputText
	exitInputText = json_object_get_string(s_config, "exitInputText");
}

Event::Event(JSON_Object*& s_event)
{
	// Check for mandatory fields
	if (!ModuleGameImporter::HandleMandatoryFields(s_event, "Event"))
		return;

	// Read text
	text = json_object_get_string(s_event, "text");

	// Read obtained conditions
	JSON_Array* s_obtainedConditions = app->gameImporter->GetLinkableArray(
		json_object_get_value(s_event, "obtainedConditions"), "obtainedConditions");
	for (int i = 0; i < json_array_get_count(s_obtainedConditions); i++)
		obtainedConditions.push_back(json_array_get_string(s_obtainedConditions, i));

	// Read removed conditions
	JSON_Array* s_removedConditions = app->gameImporter->GetLinkableArray(
		json_object_get_value(s_event, "removedConditions"), "removedConditions");
	for (int i = 0; i < json_array_get_count(s_removedConditions); i++)
		removedConditions.push_back(json_array_get_string(s_removedConditions, i));

	// Read obtained objects
	JSON_Array* s_obtainedObjects = app->gameImporter->GetLinkableArray(
		json_object_get_value(s_event, "obtainedObjects"), "obtainedObjects");
	for (int i = 0; i < json_array_get_count(s_obtainedObjects); i++)
		obtainedObjects.push_back(json_array_get_string(s_obtainedObjects, i));

	// Read sub events
	JSON_Array* s_subEvents = app->gameImporter->GetLinkableArray(
		json_object_get_value(s_event, "subEvents"), "subEvents");
	for (int i = 0; i < json_array_get_count(s_subEvents); i++) 
	{
		// Handle linkeability
		JSON_Object* s_subEvent = app->gameImporter->GetLinkableObject(json_array_get_value(s_subEvents, i), "subEvents[x]");

		// Error handling
		if (s_subEvent == nullptr) 
		{
			app->gameImporter->TerminateApplication(("No object in subEvents[" + to_string(i) + "]").c_str());
			return;
		}

		// Load SubEvent
		subEvents.push_back(SubEvent(s_subEvent));
	}


	// Read alternative events
	JSON_Array* s_alternativeEvents = app->gameImporter->GetLinkableArray(
		json_object_get_value(s_event, "alternativeEvents"), "alternativeEvents");
	for (int i = 0; i < json_array_get_count(s_alternativeEvents); i++) 
	{
		// Handle linkeability
		JSON_Object* s_alternativeEvent = app->gameImporter->GetLinkableObject(
			json_array_get_value(s_alternativeEvents, i), "alternativeEvents[x]");

		// Error handling
		if (s_alternativeEvent == nullptr)
		{
			app->gameImporter->TerminateApplication(("No object in alternativeEvents[" + to_string(i) + "]").c_str());
			return;
		}

		// Load SubEvent
		alternativeEvents.push_back(AlternativeEvent(s_alternativeEvent));
	}

	// Read saved variable
	savedVariable = SavedVariable::LoadSavedVariable(s_event, "savedVariable");
}

Event* Event::LoadEvent(JSON_Object* object, const char* eventName)
{
	Event* ret = nullptr;
	if (JSON_Object* s_event = json_object_get_object(object, eventName))
		ret = new Event(s_event);

	return ret;
}

MapEvent::MapEvent(JSON_Object* s_mapEvent): Event(s_mapEvent)
{
	// Check for mandatory fields
	if (!ModuleGameImporter::HandleMandatoryFields(s_mapEvent, "MapEvent"))
		return;

	// Read grid position
	gridPosition = json_object_get_number(s_mapEvent, "gridPosition");

	// Read navigable flag (map events are navigable by default)
	if (json_object_has_value(s_mapEvent, "navigable") == 0)
		navigable = true;
	else 
		navigable = json_object_get_boolean(s_mapEvent, "navigable");
}

MapEvent* MapEvent::LoadMapEvent(JSON_Object* object, const char* mapEventName)
{
	MapEvent* ret = nullptr;
	if (JSON_Object* s_MapEvent = json_object_get_object(object, mapEventName))
		ret = new MapEvent(s_MapEvent);

	return ret;
}

SubEvent::SubEvent(JSON_Object* s_subEvent): Event(s_subEvent)
{
	// Check for mandatory fields
	if (!ModuleGameImporter::HandleMandatoryFields(s_subEvent, "SubEvent"))
		return;

	// Read option
	option = json_object_get_string(s_subEvent, "option");

	// Read conditions
	JSON_Array* s_conditions = json_object_get_array(s_subEvent, "conditions");
	for (int i = 0; i < json_array_get_count(s_conditions); i++)
		conditions.push_back(json_array_get_string(s_conditions, i));

	// Read rejection texts
	JSON_Array* s_rejectionTexts = app->gameImporter->GetLinkableArray(
		json_object_get_value(s_subEvent, "rejectionTexts"), "rejectionTexts");
	for (int i = 0; i < json_array_get_count(s_rejectionTexts); i++) 
	{
		// Handle linkeability
		JSON_Object* s_rejectionText = app->gameImporter->GetLinkableObject(
			json_array_get_value(s_rejectionTexts, i), "s_rejectionTexts[x]");

		// Error handling
		if (s_rejectionText == nullptr)
		{
			app->gameImporter->TerminateApplication(("No object in s_rejectionTexts[" + to_string(i) + "]").c_str());
			return;
		}

		// Load SubEvent
		rejectionTexts.push_back(RejectionText(s_rejectionText));
	}
}

RejectionText::RejectionText(JSON_Object* s_rejectionText)
{
	// Check for mandatory fields
	if (!ModuleGameImporter::HandleMandatoryFields(s_rejectionText, "RejectionText"))
		return;

	// Read conditions
	JSON_Array* s_conditions = json_object_get_array(s_rejectionText, "conditions");
	for (int i = 0; i < json_array_get_count(s_conditions); i++)
		conditions.push_back(json_array_get_string(s_conditions, i));

	// Read text
	text = json_object_get_string(s_rejectionText, "text");
}

AlternativeEvent::AlternativeEvent(JSON_Object* s_alternativeEvent)
{
	// Check for mandatory fields
	if (!ModuleGameImporter::HandleMandatoryFields(s_alternativeEvent, "AlternativeEvent"))
		return;

	// Read conditions
	JSON_Array* s_conditions = json_object_get_array(s_alternativeEvent, "conditions");
	for (int i = 0; i < json_array_get_count(s_conditions); i++)
		conditions.push_back(json_array_get_string(s_conditions, i));

	// Read alternative event
	alternative = MapEvent::LoadMapEvent(s_alternativeEvent, "alternative");
}

SavedVariable::SavedVariable(JSON_Object* s_savedVariable)
{
	// Check for mandatory fields
	if (!ModuleGameImporter::HandleMandatoryFields(s_savedVariable, "SavedVariable"))
		return;

	// Read key
	key = json_object_get_string(s_savedVariable, "key");

	// Read confirmation text
	confirmationText = json_object_get_string(s_savedVariable, "confirmationText");

	// Read success txt
	successText = json_object_get_string(s_savedVariable, "successText");

	// Read next event
	nextEvent = Event::LoadEvent(s_savedVariable, "nextEvent");
}

SavedVariable* SavedVariable::LoadSavedVariable(JSON_Object* object, const char* savedVariableName)
{
	SavedVariable* ret = nullptr;
	if (JSON_Object* s_SavedVariable = json_object_get_object(object, savedVariableName))
		ret = new SavedVariable(s_SavedVariable);

	return ret;
}