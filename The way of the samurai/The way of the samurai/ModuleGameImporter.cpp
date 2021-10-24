#include "ModuleGameImporter.h"
#include "Application.h"
#include "Third Party/parson.h"

bool ModuleGameImporter::Init()
{
	bool ret = true;
	// TODO: Look for a dynamic way to configure the name of the file being read as the game
	// Extract map events from file. 
	JSON_Value* rawFile = json_parse_file("The way of the samurai.json");
	JSON_Object* game = json_value_get_object(rawFile);

	// Read all map events
	JSON_Array* s_mapEvents = json_object_get_array(game, "mapEvents");
	for (int i = 0; i < json_array_get_count(s_mapEvents); i++) 
		mapEvents.push_back(MapEvent(json_array_get_object(s_mapEvents, i)));

	// Read config
	if(JSON_Object* s_config = json_object_get_object(game, "config"))
		config = new Config(s_config);
	else 
	{
		app->logFatalError("Couldn't load config file, terminating aplication");
		ret = false;
	}

	// Clean JSON after loading it
	json_value_free(rawFile);

	return ret;
}

bool ModuleGameImporter::CleanUp()
{
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

Config::Config(JSON_Object* s_config)
{
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
	// Read grid position
	gridPosition = json_object_get_number(s_mapEvent, "gridPosition");

	// Read navigable flag
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
	// Read conditions
	JSON_Array* s_conditions = json_object_get_array(s_rejectionText, "conditions");
	for (int i = 0; i < json_array_get_count(s_conditions); i++)
		conditions.push_back(json_array_get_string(s_conditions, i));

	// Read text
	text = json_object_get_string(s_rejectionText, "text");
}

AlternativeEvent::AlternativeEvent(JSON_Object* s_alternativeEvent)
{
	// Read conditions
	JSON_Array* s_conditions = json_object_get_array(s_alternativeEvent, "conditions");
	for (int i = 0; i < json_array_get_count(s_conditions); i++)
		conditions.push_back(json_array_get_string(s_conditions, i));

	// Read alternative event
	alternative = MapEvent::LoadMapEvent(s_alternativeEvent, "alternative");
}

SavedVariable::SavedVariable(JSON_Object* s_saveVariable)
{
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
