#include "ModuleGameImporter.h"
#include "Application.h"
#include "Third Party/parson.h"

bool ModuleGameImporter::Init()
{
	// TODO: Look for a dynamic way to configure the name of the file being read as the game
	// Extract map events from file. 
	JSON_Value* rawFile = json_parse_file("The way of the samurai.json");
	JSON_Object* game = json_value_get_object(rawFile);
	JSON_Array* s_mapEvents = json_object_get_array(game, "mapEvents");

	// Read all map events
	for (int i = 0; i < json_array_get_count(s_mapEvents); i++) 
		mapEvents.push_back(MapEvent(json_array_get_object(s_mapEvents, i)));


	return true;
}

Event::Event(JSON_Object* s_event)
{
	// Read text
	text = json_object_get_string(s_event, "text");

	// Read obtained conditions
	JSON_Array* s_obtainedConditions = json_object_get_array(s_event, "obtainedConditions");
	for (int i = 0; i < json_array_get_count(s_obtainedConditions); i++)
		obtainedConditions.push_back(json_array_get_string(s_obtainedConditions, i));

	// Read obtained objects
	JSON_Array* s_obtainedObjects = json_object_get_array(s_event, "obtainedObjects");
	for (int i = 0; i < json_array_get_count(s_obtainedObjects); i++)
		obtainedObjects.push_back(json_array_get_string(s_obtainedObjects, i));

	// Read sub events
	JSON_Array* s_subEvents = json_object_get_array(s_event, "subEvents");
	for (int i = 0; i < json_array_get_count(s_subEvents); i++)
		subEvents.push_back(SubEvent(json_array_get_object(s_subEvents, i)));
}

MapEvent::MapEvent(JSON_Object* s_mapEvent): Event(s_mapEvent)
{
	// Read grid position
	gridPosition = json_object_get_number(s_mapEvent, "gridPosition");
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
