#pragma once
#include "Module.h"
#include <string>
#include <vector>
using namespace std;

// Forward declaration
typedef struct json_object_t JSON_Object;
typedef struct json_array_t  JSON_Array;
typedef struct json_value_t  JSON_Value;

struct Config;
struct Event;
struct MapEvent;
struct SubEvent;
struct AlternativeEvent;
struct RejectionText;
struct SavedVariable;

class ModuleGameImporter: public Module
{
// Public methods
public:
	// Module inherited methods
	bool Init() override;
	bool CleanUp() override;

	// Returns "true" if the object has all the mandatory fields and "false" otherwise. Additionally, handles the absence of mandatory
	// fields as an error
	static bool HandleMandatoryFields(JSON_Object* jsonObject, const char* objectType);
	// Returns en empty string if the object has all the mandatory fields and the missing mandatory field otherwise
	static string HasFields(JSON_Object* jsonObject, vector<string>& mandatoryFields);

// Public attributes
public:
	// Map events 
	vector<MapEvent> mapEvents;
	// Configuration
	Config* config = nullptr;


// Private methods
private:

	// ----------LOGIC ENCAPSULATORS (only for readibility, this functions shouldn't be used more than once)
	// Initializes static mandatory fields for all the objects
	void InitMandatoryFields();
	// Cleans up memory for static mandatory fields in all the objects
	void CleanUpMandatoryFields();
	

	// Gets an array from an object accounting for linkability
	JSON_Array* GetLinkableArray(JSON_Object* object, const char* arrayName);

// Private attributes
private:
	// If set to "false" at any time during loading it will terminate the application
	static bool correctLoading;
	// Stores all the linked files in the game JSON to clean up afterwards
	static vector<JSON_Value*> linkedFiles;

};

// Abstrct structure for all linkable objects to inherit from
struct Linkable 
{
	Linkable(JSON_Object*& s_linkable);
};

// Game parameters
struct Config
{
	// Public methods
public:
	// C++ pesao
	Config() {};
	Config(JSON_Object* s_config);

	// Public attributes
public:
	int gridRowLength = 0;
	int initialPosition = 0;
	string initialText;
	string defaultSubEventRejectionMessage;

	// Mandatory fields when loading the object
	static vector<string> mandatoryFields;
};

// Event base class
struct Event
{
// Public methods
public:
	// C++ pesao
	Event() {};
	Event(JSON_Object* s_event);

	// Error handling functions for serialization
	static Event* LoadEvent(JSON_Object* object, const char* eventName);
// Public attributes
public:
	string text;
	vector<string> obtainedConditions;
	vector<string> removedConditions;
	vector<string> obtainedObjects;
	vector<SubEvent> subEvents;
	vector<AlternativeEvent> alternativeEvents;
	SavedVariable* savedVariable = nullptr;

	// Mandatory fields when loading the object
	static vector<string> mandatoryFields;
};

// Event that can happen due to a map displacement
struct MapEvent : public Event
{
// Public methods
public:
	MapEvent(JSON_Object* s_mapEvent);

	// Error handling function for serialization
	static MapEvent* LoadMapEvent(JSON_Object* object, const char* mapEventName);
// Public attributes
public:
	int gridPosition = 0;
	bool navigable = false;

	// Mandatory fields when loading the object
	static vector<string> mandatoryFields;
};

// Event that can happen within an event
struct SubEvent : Event 
{
// Public methods
public:
	SubEvent(JSON_Object* s_subEvent);

// Public attributes
public:
	string option;
	vector<string> conditions;
	vector<RejectionText> rejectionTexts;

	// Mandatory fields when loading the object
	static vector<string> mandatoryFields;
};

// Alternative event that happens when certain conditions are met
struct AlternativeEvent
{
// Public methods
public:
	AlternativeEvent(JSON_Object* s_alternativeEvent);

// Public attributes
public:
	vector<string> conditions;
	MapEvent* alternative = nullptr;

	// Mandatory fields when loading the object
	static vector<string> mandatoryFields;
};

// Rejection text to show depending on the conditions that are met by the player
struct RejectionText
{
// Public methods
public:
	RejectionText(JSON_Object* s_subEvent);

// Public attributes
public:
	vector<string> conditions;
	string text;

	// Mandatory fields when loading the object
	static vector<string> mandatoryFields;
};

// Data needed to save 
struct SavedVariable
{
// Public methods
public:
	SavedVariable() {};
	SavedVariable(JSON_Object* s_saveVariable);

	// Error handling function for serialization
	static SavedVariable* LoadSavedVariable(JSON_Object* object, const char* saveVariableName);
// Public attributes
public:
	string key;
	string confirmationText;
	string successText;
	Event* nextEvent = nullptr;

	// Mandatory fields when loading the object
	static vector<string> mandatoryFields;
};
