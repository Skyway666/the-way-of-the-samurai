#pragma once
#include "Module.h"
#include <string>
#include <vector>
using namespace std;

// Forward declaration
typedef struct json_object_t JSON_Object;

struct Event;
struct MapEvent;
struct SubEvent;
struct AlternativeEvent;
struct RejectionText;

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
	int gridRowLength;
	int initialPosition;
	string initialText;
	bool savePlayerName;
	string savePlayerNameText;
	string defaultSubEventRejectionMessage;
};


class ModuleGameImporter: public Module
{
// Public methods
public:
	// Module inherited methods
	bool Init() override;

// Public attributes
public:
	// Map events 
	vector<MapEvent> mapEvents;
	// Configuration
	Config config;
};

// Event base class
struct Event 
{
// Public methods
public:
	// C++ pesao
	Event() {};
	Event(JSON_Object* s_event);
// Public attributes
public:
	string text;
	vector<string> obtainedConditions;
	vector<string> removedConditions;
	vector<string> obtainedObjects;
	vector<SubEvent> subEvents;
	vector<AlternativeEvent> alternativeEvents;
};

// Event that can happen due to a map displacement
struct MapEvent : public Event
{
// Public methods
public:
	MapEvent(JSON_Object* s_mapEvent);

// Public attributes
public:
	int gridPosition;
	bool navigable;
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
	Event alternative;
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
};

