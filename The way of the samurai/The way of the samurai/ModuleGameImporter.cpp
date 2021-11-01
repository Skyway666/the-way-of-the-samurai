#include "ModuleGameImporter.h"
#include "Application.h"
#include "Third Party/parson.h"

// Static variables declaration
vector<string> Config::mandatoryFields;
vector<string> Event::mandatoryFields;
vector<string> MapEvent::mandatoryFields;
vector<string> SubEvent::mandatoryFields;
vector<string> AlternativeEvent::mandatoryFields;
vector<string> RejectionText::mandatoryFields;
vector<string> SavedVariable::mandatoryFields;
bool ModuleGameImporter::correctLoading = true;
vector<JSON_Value*> ModuleGameImporter::linkedFiles;

bool ModuleGameImporter::Init()
{
	// Module name
	name = "Game Importer";

	// Set mandatory fields for every object
	InitMandatoryFields();

	// TODO: Look for a dynamic way to configure the name of the file being read as the game
	// Read file
	JSON_Value* rawFile = json_parse_file("Game.json");
	// Error handling for missing file
	if (rawFile == nullptr) 
	{
		app->logFatalError("Couldn't load game file: 'The way of the samurai.json'");
		return false;
	}

	// Read root object
	JSON_Object* game = json_value_get_object(rawFile);
	// Error handling for missing root object in file
	if (game == nullptr) 
	{
		app->logFatalError("No root object in the game file");
		return false;
	}

	// Read all map events
	JSON_Array* s_mapEvents = GetLinkableArray(game, "mapEvents");
	// Error handling for map events
	if (s_mapEvents == nullptr) 
	{
		app->logFatalError("No 'mapEvents' array in root object");
		return false;
	}
	// Parse each map event
	for (int i = 0; i < json_array_get_count(s_mapEvents); i++) 
		mapEvents.push_back(MapEvent(json_array_get_object(s_mapEvents, i)));

	// Read config
	JSON_Object* s_config = json_object_get_object(game, "config");
	// Error handling for config file
	if(s_config == nullptr)
	{
		app->logFatalError("Couldn't load config file, terminating aplication");
		return false;
	}
	// Parse config file
	config = new Config(s_config);

	// Clean game file
	json_value_free(rawFile);
	// Clean linked files
	for (vector<JSON_Value*>::iterator it = ModuleGameImporter::linkedFiles.begin();
		it != ModuleGameImporter::linkedFiles.end();
		it++)
		json_value_free(*it);

	return correctLoading;
}

bool ModuleGameImporter::CleanUp()
{
	// Clean up mandatory fields
	CleanUpMandatoryFields();

	// Clean config
	delete config;
	config = nullptr;

	// Clean map events
	for (vector<MapEvent>::iterator it = mapEvents.begin(); it != mapEvents.end(); it++) 
	{
		// Clean saved variable in map event
		if ((*it).savedVariable != nullptr) 
		{
			// Clean nextEvent in saved variable
			if ((*it).savedVariable->nextEvent != nullptr)
				delete (*it).savedVariable->nextEvent;

			delete (*it).savedVariable;
		}

		// Clean alternative events
		for (vector<AlternativeEvent>::iterator altIt = (*it).alternativeEvents.begin(); 
			altIt != (*it).alternativeEvents.end(); 
			altIt++)
		{
			// Clean alternative in alternativeEvents
			if ((*altIt).alternative != nullptr)
				delete (*altIt).alternative;
		}

		(*it).alternativeEvents.clear();
	}

	mapEvents.clear();
	mapEvents.shrink_to_fit();

	// I don't know what could go wrong while cleaning up
	return true;
}

bool ModuleGameImporter::HandleMandatoryFields(JSON_Object* jsonObject, const char* objectType)
{
	// Get referece to the mandatory fields depending on the object type
	vector<string>* mandatoryFields = nullptr;

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

	// Check if the jsonObject has all the mandatory 
	string missingField("");
	if(mandatoryFields)
	 missingField = HasFields(jsonObject, *mandatoryFields);

	// Return true if no missing field was found and false otherwise
	bool ret = missingField.empty();

	// If the object is missing a field
	if (!ret) 
	{
		// Inform the user about the missing mandatory field
		string errorLog = ("Object of type '" + string(objectType) + "' is missing the mandatory field " + missingField);
		app->logFatalError(errorLog.c_str());
		// Inform the Game Importer of an incorrect loading
		correctLoading = false;
	}

	return ret;
}

string ModuleGameImporter::HasFields(JSON_Object* jsonObject, vector<string>& fields)
{
	// Iterate mandatory fields
	for (vector<string>::iterator it = fields.begin(); it != fields.end(); it++) 
		// Check if the object has the mandatory value
		if (json_object_has_value(jsonObject, (*it).c_str()) == 0) 
			// Return the missing mandatory field
			return *it;

	// No mandatory field was missing
	return string();
}

void ModuleGameImporter::InitMandatoryFields()
{
	// Config
	Config::mandatoryFields.push_back("gridRowLength");
	Config::mandatoryFields.push_back("initialPosition");
	Config::mandatoryFields.push_back("initialText");
	Config::mandatoryFields.push_back("defaultSubEventRejectionMessage");

	// Event
	Event::mandatoryFields.push_back("text");

	// SubEvent
	SubEvent::mandatoryFields.push_back("option");

	// Rejection text
	RejectionText::mandatoryFields.push_back("text");

	// Alternative event
	AlternativeEvent::mandatoryFields.push_back("conditions");
	AlternativeEvent::mandatoryFields.push_back("alternative");

	// Saved variable
	SavedVariable::mandatoryFields.push_back("key");
	SavedVariable::mandatoryFields.push_back("confirmationText");
	SavedVariable::mandatoryFields.push_back("successText");
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

JSON_Array* ModuleGameImporter::GetLinkableArray(JSON_Object* object, const char* arrayName)
{
	JSON_Value* rootArrayValue = json_object_get_value(object, arrayName);

	// The object did not contain the value
	if (rootArrayValue == nullptr)
		return nullptr;

	JSON_Array* ret = nullptr;
	// The root object already contains the array
	if (json_value_get_type(rootArrayValue) == json_value_type::JSONArray)
		ret = json_value_get_array(rootArrayValue);
	// The root object contains a link to the file that contains the array
	else if (json_value_get_type(rootArrayValue) == json_value_type::JSONString) 
	{
		// Load linked file
		const char* linkedArrayPath = json_value_get_string(rootArrayValue);
		JSON_Value* rawArrayFile = json_parse_file(linkedArrayPath);
		// Error handling
		if (rawArrayFile == nullptr) 
		{
			// Inform the user the linked file could not be loaded
			app->logFatalError(("File for linked array '" + string(arrayName) + "' with path " + linkedArrayPath + " could not be found").c_str());
			return nullptr;
		}
		// Add loaded files for later cleaning
		ModuleGameImporter::linkedFiles.push_back(rawArrayFile);

		// Load array from file
		ret = json_value_get_array(rawArrayFile);
		// Error handling
		if (ret == nullptr) 
		{
			app->logFatalError(("File for linked array '" + string(arrayName) + "' with path " + linkedArrayPath + " didn't contain an array as root value").c_str());
		}
	}


	return ret;
}

Linkable::Linkable(JSON_Object*& s_linkable)
{
	// Check if the object has a link
	if (json_object_has_value(s_linkable, "link") == 1) 
	{
		// TODO: Load the object with error handling and override the s_linkable parameter value
		// remember to add the loaded JSON_Value into the "ModuleGameImporter::linkedFiles" vector
	}
}

Config::Config(JSON_Object* s_config)
{
	// Check for mandatory fields
	if (!ModuleGameImporter::HandleMandatoryFields(s_config, "Config"))
		return;

	// Read grid row length
	gridRowLength = json_object_get_number(s_config, "gridRowLength");

	// Read initial position
	initialPosition = json_object_get_number(s_config, "initialPosition");

	// Read initial text
	initialText = json_object_get_string(s_config, "initialText");

	// Read defaultSubEventRejectionMessage
	defaultSubEventRejectionMessage = json_object_get_string(s_config, "defaultSubEventRejectionMessage");
}

Event::Event(JSON_Object* s_event)
{
	// Check for mandatory fields
	if (!ModuleGameImporter::HandleMandatoryFields(s_event, "Event"))
		return;

	// Read text
	text = json_object_get_string(s_event, "text");

	// Read obtained conditions
	JSON_Array* s_obtainedConditions = json_object_get_array(s_event, "obtainedConditions");
	for (int i = 0; i < json_array_get_count(s_obtainedConditions); i++)
		obtainedConditions.push_back(json_array_get_string(s_obtainedConditions, i));

	// Read removed conditions
	JSON_Array* s_removedConditions = json_object_get_array(s_event, "removedConditions");
	for (int i = 0; i < json_array_get_count(s_removedConditions); i++)
		removedConditions.push_back(json_array_get_string(s_removedConditions, i));

	// Read obtained objects
	JSON_Array* s_obtainedObjects = json_object_get_array(s_event, "obtainedObjects");
	for (int i = 0; i < json_array_get_count(s_obtainedObjects); i++)
		obtainedObjects.push_back(json_array_get_string(s_obtainedObjects, i));

	// Read sub events
	JSON_Array* s_subEvents = json_object_get_array(s_event, "subEvents");
	for (int i = 0; i < json_array_get_count(s_subEvents); i++)
		subEvents.push_back(SubEvent(json_array_get_object(s_subEvents, i)));

	// Read alternative events
	JSON_Array* s_alternativeEvents = json_object_get_array(s_event, "alternativeEvents");
	for (int i = 0; i < json_array_get_count(s_alternativeEvents); i++)
		alternativeEvents.push_back(AlternativeEvent(json_array_get_object(s_alternativeEvents, i)));

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
	JSON_Array* s_rejectionTexts = json_object_get_array(s_subEvent, "rejectionTexts");
	for (int i = 0; i < json_array_get_count(s_rejectionTexts); i++)
		rejectionTexts.push_back(RejectionText(json_array_get_object(s_rejectionTexts, i)));
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

SavedVariable::SavedVariable(JSON_Object* s_saveVariable)
{
	// Check for mandatory fields
	if (!ModuleGameImporter::HandleMandatoryFields(s_saveVariable, "SavedVariable"))
		return;

	// Read key
	key = json_object_get_string(s_saveVariable, "key");

	// Read confirmation text
	confirmationText = json_object_get_string(s_saveVariable, "confirmationText");

	// Read success txt
	successText = json_object_get_string(s_saveVariable, "successText");

	// Read next event
	nextEvent = Event::LoadEvent(s_saveVariable, "nextEvent");
}

SavedVariable* SavedVariable::LoadSavedVariable(JSON_Object* object, const char* savedVariableName)
{
	SavedVariable* ret = nullptr;
	if (JSON_Object* s_SavedVariable = json_object_get_object(object, savedVariableName))
		ret = new SavedVariable(s_SavedVariable);

	return ret;
}
