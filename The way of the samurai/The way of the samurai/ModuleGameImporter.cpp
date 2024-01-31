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
	JSON_Value* rawFile = json_parse_file_with_comments("Game.json");
	// Error handling for missing file
	if (rawFile == nullptr) 
	{
		app->LogFatalError("Couldn't load game file: 'Game.json'");
		return false;
	}

	// Read root object
	JSON_Object* game = json_value_get_object(rawFile);
	// Error handling for missing root object in file
	if (game == nullptr) 
	{
		app->LogFatalError("No root object in the game file");
		return false;
	}

	// Read all map events
	JSON_Array* s_mapEvents = GetLinkableArray(game, "mapEvents");
	// Error handling for map events
	if (s_mapEvents == nullptr) 
	{
		app->LogFatalError("No 'mapEvents' array in root object");
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
		app->LogFatalError("Couldn't load config file, terminating aplication");
		return false;
	}
	// Parse config file
	config = new Config(s_config);

	// Clean game file
	json_value_free(rawFile);
	// Clean linked files
	for (JSON_Value* jsonValue : linkedFiles)
		json_value_free(jsonValue);

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
		string errorLog = ("Object of type '" + string(objectType) + "' is missing the mandatory field '" + missingField + "'");
		app->LogFatalError(errorLog.c_str());
		// Inform the Game Importer of an incorrect loading
		correctLoading = false;
	}

	return ret;
}

string ModuleGameImporter::HasFields(JSON_Object* jsonObject, vector<string>& fields)
{
	// Iterate mandatory fields
	for (string& field : fields) 
		// Check if the object has the mandatory value
		if (json_object_has_value(jsonObject, field.c_str()) == 0)
			// Return the missing mandatory field
			return field;

	// No mandatory field was missing
	return string();
}

void ModuleGameImporter::InitMandatoryFields()
{
	// Config
	Config::mandatoryFields.push_back("gridRowLength");
	Config::mandatoryFields.push_back("initialPosition");
	Config::mandatoryFields.push_back("initialText");
	Config::mandatoryFields.push_back("defaultSubEventRejectionText");
	Config::mandatoryFields.push_back("displayOptionsText");
	Config::mandatoryFields.push_back("backToMapText");
	Config::mandatoryFields.push_back("invalidOptionText");
	Config::mandatoryFields.push_back("languageSelectedText");
	Config::mandatoryFields.push_back("optionsMenuIntroductionText");
	Config::mandatoryFields.push_back("availableObjectsText");
	Config::mandatoryFields.push_back("currentConditionsText");
	Config::mandatoryFields.push_back("exitGameText");
	Config::mandatoryFields.push_back("helpText");
	Config::mandatoryFields.push_back("yesInputText");
	Config::mandatoryFields.push_back("noInputText");
	Config::mandatoryFields.push_back("northInputText");
	Config::mandatoryFields.push_back("southInputText");
	Config::mandatoryFields.push_back("eastInputText");
	Config::mandatoryFields.push_back("westInputText");
	Config::mandatoryFields.push_back("optionsInputText");
	Config::mandatoryFields.push_back("reenterInputText");
	Config::mandatoryFields.push_back("objectsInputText");
	Config::mandatoryFields.push_back("conditionsInputText");
	Config::mandatoryFields.push_back("helpInputText");
	Config::mandatoryFields.push_back("languageInputText");
	Config::mandatoryFields.push_back("resumeInputText");
	Config::mandatoryFields.push_back("exitInputText");

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
		JSON_Value* rawArrayFile = json_parse_file_with_comments(linkedArrayPath);
		// Error handling
		if (rawArrayFile == nullptr) 
		{
			// Inform the user about the error
			app->LogFatalError(("File for linked array '" + string(arrayName) + "' with path '" + linkedArrayPath + "' could not be found").c_str());
			return nullptr;
		}
		// Add loaded files for later cleaning
		ModuleGameImporter::linkedFiles.push_back(rawArrayFile);

		// Load array from file
		ret = json_value_get_array(rawArrayFile);
		// Error handling
		if (ret == nullptr) 
		{
			// Inform the user about the error
			app->LogFatalError(("File for linked array '" + string(arrayName) + "' with path '" + linkedArrayPath + "' didn't contain an array as root value").c_str());
		}
	}

	return ret;
}

Linkable::Linkable(JSON_Object*& s_linkable, const char* objectName)
{
	// Check if the object has a link
	if (json_object_has_value(s_linkable, "link") == 1) 
	{
		// Load linked file
		const char* linkedObjectPath = json_object_get_string(s_linkable, "link");
		JSON_Value* rawObjectFile = json_parse_file_with_comments(linkedObjectPath);
		// Error handling
		if (rawObjectFile == nullptr) 
		{
			// Inform the user about the error
			app->LogFatalError(("File for linked object '" + string(objectName) + "' with path '" + linkedObjectPath + "' could not be found").c_str());
			return;
		}
		// Add loaded files for later cleaning
		ModuleGameImporter::linkedFiles.push_back(rawObjectFile);

		// Load object from file
		JSON_Object* linkedObject = json_value_get_object(rawObjectFile);
		if (linkedObject == nullptr) 
		{
			// Inform the user about the error
			app->LogFatalError(("File for linked object '" + string(objectName) + "' with path '" + linkedObjectPath + "' didn't contain an object as root value").c_str());
			return;
		}

		// Replace serialized object for further parsing
		s_linkable = linkedObject;
	}
}

Config::Config(JSON_Object* s_config): Linkable(s_config, "config")
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

Event::Event(JSON_Object*& s_event) : Linkable(s_event, "event")
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

RejectionText::RejectionText(JSON_Object* s_rejectionText): Linkable(s_rejectionText, "rejectionText")
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

AlternativeEvent::AlternativeEvent(JSON_Object* s_alternativeEvent): Linkable(s_alternativeEvent, "alternativeEvent")
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

SavedVariable::SavedVariable(JSON_Object* s_savedVariable): Linkable(s_savedVariable, "savedVariable")
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
