#pragma once
#include "Module.h"
#include <string>
#include <vector>
using namespace std;

// Forward declaration
typedef struct json_object_t JSON_Object;
typedef struct json_array_t  JSON_Array;
typedef struct json_value_t  JSON_Value;
typedef int JSON_Value_Type;

struct JSONFieldData;
struct Config;
struct Event;
struct MapEvent;
struct SubEvent;
struct AlternativeEvent;
struct SavedVariable;

class ModuleGameImporter: public Module
{
// Public methods
public:
	// Module inherited methods
	void Init() override;
	void CleanUp() override;

	// Returns "true" if the object has all the mandatory fields and "false" otherwise. Additionally, handles the absence of mandatory
	// fields as an error
	static bool HandleMandatoryFields(JSON_Object* jsonObject, const char* objectType);
	// Returns en empty JSONFieldData if the object has all the mandatory fields and the missing mandatory field otherwise
	static JSONFieldData HasFields(JSON_Object* jsonObject, vector<JSONFieldData>& mandatoryFields);
	// Get a JSON_Object* from a linkeable value
	JSON_Object* GetLinkableObject(JSON_Value* linkeableValue, const char* objectName);
	// Get a JSON_Array* from a linkeable value
	JSON_Array* GetLinkableArray(JSON_Value* linkeableValue, const char* arrayName);
	// Get a JSON_Object* from an array accounting for linkeability
	JSON_Object* GetLinkeableObjectFromArray(JSON_Array* s_array, int index, const char* arrayName);

// Public attributes
public:
	// Map events 
	vector<MapEvent> mapEvents;
	// Configuration
	Config* config = nullptr;
	// Stores all the linked files in the game JSON to clean up afterwards
	static vector<JSON_Value*> linkedFiles;

// Private methods
private:

	// ----------LOGIC ENCAPSULATORS (only for readibility, this functions shouldn't be used more than once)
	// Initializes static mandatory fields for all the objects
	void InitMandatoryFields();
	// Cleans up memory for static mandatory fields in all the objects
	void CleanUpMandatoryFields();

	// Checks if a JSON_Value* is a string and substitutes it for the value in the string's path file if possible
	void ProcessLinkableValue(JSON_Value*& linkeableValue, const char* valueName, JSON_Value_Type valueType);
};

struct JSONFieldData 
{
	string name = "";
	JSON_Value_Type type = 1;

	JSONFieldData() {};
	JSONFieldData(string name, JSON_Value_Type type);
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

	// Output
	string initialText;
	string defaultSubEventRejectionText;
	string displayOptionsText;
	string backToMapText;
	string invalidOptionText;
	string invalidCharacterText;
	string languageSelectedText;
	string optionsMenuIntroductionText;
	string availableObjectsText;
	string currentConditionsText;
	string savedText;
	string exitGameText;
	string helpText;
	string languageChoosingQuestionText;

	// Input
	string yesInputText;
	string noInputText;
	string northInputText;
	string southInputText;
	string eastInputText;
	string westInputText;
	string optionsInputText;
	string reenterInputText;
	string objectsInputText;
	string conditionsInputText;
	string saveInputText;
	string helpInputText;
	string languageInputText;
	string resumeInputText;
	string exitInputText;

	// Mandatory fields when loading the object
	static vector<JSONFieldData> mandatoryFields;
};

// Event base class
struct Event
{
// Public methods
public:
	// C++ pesao
	Event() {};
	Event(JSON_Object*& s_event);

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
	static vector<JSONFieldData> mandatoryFields;
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
	static vector<JSONFieldData> mandatoryFields;
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

	// Mandatory fields when loading the object
	static vector<JSONFieldData> mandatoryFields;
};

// Alternative event that happens when certain conditions are met
struct AlternativeEvent : MapEvent
{
// Public methods
public:
	AlternativeEvent(JSON_Object* s_alternativeEvent);

// Public attributes
public:
	vector<string> conditions;

	// Mandatory fields when loading the object
	static vector<JSONFieldData> mandatoryFields;
};

// Data needed to save 
struct SavedVariable
{
// Public methods
public:
	SavedVariable() {};
	SavedVariable(JSON_Object* s_savedVariable);

	// Error handling function for serialization
	static SavedVariable* LoadSavedVariable(JSON_Object* object, const char* savedVariableName);
// Public attributes
public:
	string key;
	string confirmationText;
	string successText;
	Event* nextEvent = nullptr;

	// Mandatory fields when loading the object
	static vector<JSONFieldData> mandatoryFields;
};
